/**
 * @file
 * Test suite for vkrenderer swapchain selection
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <vulkan/vulkan_core.h>
#include "vkrenderer.h"

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
	VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
	VkSurfaceCapabilitiesKHR *caps)
{
	return (VkResult)mock(physicalDevice, surface, caps);
}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(
	VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
	uint32_t *pSurfaceFormatCount, VkSurfaceFormatKHR *pSurfaceFormats)
{
	return (VkResult)mock(physicalDevice, surface, pSurfaceFormatCount,
			      pSurfaceFormats);
}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(
	VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
	uint32_t *pPresentModeCount, VkPresentModeKHR *pPresentModes)
{
	return (VkResult)mock(physicalDevice, surface, pPresentModeCount,
			      pPresentModes);
}

Ensure(configure_fails_when_no_memory_for_surface_capabilities)
{
	struct vkrenderer rdr;
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_return(VK_INCOMPLETE));
	int result = vkrenderer_configure_swapchain(&rdr);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_fails_when_no_memory_for_surface_formats)
{
	struct vkrenderer rdr;
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_return(VK_INCOMPLETE));
	int result = vkrenderer_configure_surface_format(&rdr);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_fails_when_no_surface_formats_available)
{
	struct vkrenderer rdr;
	uint32_t nfmts = 0;
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(nfmts)));
	int result = vkrenderer_configure_surface_format(&rdr);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_fails_when_no_suitable_surface_formats)
{
	struct vkrenderer rdr;
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_return(VK_INCOMPLETE));
	int result = vkrenderer_configure_swapchain(&rdr);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_fails_when_no_suitable_present_modes)
{
	struct vkrenderer rdr;
	VkSurfaceFormatKHR fmts[] = {
		{
			.format = VK_FORMAT_UNDEFINED,
		},
	};
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(nfmts)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(*fmts) * nfmts));
	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       will_return(VK_INCOMPLETE));
	int result = vkrenderer_configure_swapchain(&rdr);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_selects_bgra_unorm_srgb_format_when_possible)
{
	struct vkrenderer rdr;
	VkSurfaceFormatKHR fmts[] = {
		{
			.format = VK_FORMAT_UNDEFINED,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		},
	};
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(nfmts)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(*fmts) * nfmts));
	int result = vkrenderer_configure_surface_format(&rdr);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.srf_format.format,
		    is_equal_to(VK_FORMAT_B8G8R8A8_UNORM));
	assert_that(rdr.srf_format.colorSpace,
		    is_equal_to(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR));
}

Ensure(configure_finds_bgra_unorm_srgb_format_in_available)
{
	struct vkrenderer rdr;
	VkSurfaceFormatKHR fmts[] = {
		{
			.format = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		},
		{
			.format = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		},
		{
			.format = VK_FORMAT_B8G8R8A8_UNORM,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		},
	};
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(nfmts)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(*fmts) * nfmts));
	int result = vkrenderer_configure_surface_format(&rdr);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.srf_format.format,
		    is_equal_to(VK_FORMAT_B8G8R8A8_UNORM));
	assert_that(rdr.srf_format.colorSpace,
		    is_equal_to(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR));
}

Ensure(configure_selects_first_when_no_bgra_unorm_srgb_format)
{
	struct vkrenderer rdr;
	VkSurfaceFormatKHR fmts[] = {
		{
			.format = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
			.colorSpace = VK_COLOR_SPACE_DCI_P3_LINEAR_EXT,
		},
		{
			.format = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		},
		{
			.format = VK_FORMAT_R8_SRGB,
			.colorSpace = VK_COLOR_SPACE_DCI_P3_LINEAR_EXT,
		},
	};
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(nfmts)),
	       will_set_contents_of_parameter(pSurfaceFormats, fmts,
					      sizeof(*fmts) * nfmts));
	int result = vkrenderer_configure_surface_format(&rdr);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.srf_format.format, is_equal_to(fmts[0].format));
	assert_that(rdr.srf_format.colorSpace, is_equal_to(fmts[0].colorSpace));
}

Ensure(configure_fails_when_no_memory_for_surface_modes)
{
	struct vkrenderer rdr;
	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       will_return(VK_INCOMPLETE));
	int result = vkrenderer_configure_surface_present_mode(&rdr);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_fails_when_no_surface_modes_available)
{
	struct vkrenderer rdr;
	uint32_t nmodes = 0;
	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       will_set_contents_of_parameter(pPresentModeCount, &nmodes,
					      sizeof(nmodes)),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure_surface_present_mode(&rdr);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_selects_fifo_mode)
{
	struct vkrenderer rdr;
	VkPresentModeKHR modes[] = {
		VK_PRESENT_MODE_IMMEDIATE_KHR,
		VK_PRESENT_MODE_FIFO_KHR,
	};
	uint32_t nmodes = ARRAY_SIZE(modes);
	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       will_set_contents_of_parameter(pPresentModeCount, &nmodes,
					      sizeof(nmodes)),
	       will_set_contents_of_parameter(pPresentModes, modes,
					      sizeof(*modes) * nmodes),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure_surface_present_mode(&rdr);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.srf_mode, is_equal_to(VK_PRESENT_MODE_FIFO_KHR));
}

Ensure(configure_selects_suitable_swapchain)
{
	struct vkrenderer rdr;
	VkSurfaceCapabilitiesKHR caps = {
		.minImageCount = 1,
		.maxImageCount = 1,
		.currentExtent = { .width = 640, .height = 480 },
		.minImageExtent = { .width = 640, .height = 480 },
		.maxImageExtent = { .width = 640, .height = 480 },
		.maxImageArrayLayers = 1,
		.supportedTransforms = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.currentTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.supportedCompositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.supportedUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
	};
	VkSurfaceFormatKHR fmts[] = {
		{
			.format = VK_FORMAT_UNDEFINED,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		},
	};
	uint32_t nfmts = ARRAY_SIZE(fmts);
	VkPresentModeKHR modes[] = { VK_PRESENT_MODE_FIFO_KHR };
	uint32_t nmodes = ARRAY_SIZE(modes);
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_set_contents_of_parameter(caps, &caps, sizeof(caps)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(nfmts)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(*fmts) * nfmts));
	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       will_set_contents_of_parameter(pPresentModeCount, &nmodes,
					      sizeof(nmodes)),
	       will_set_contents_of_parameter(pPresentModes, modes,
					      sizeof(*modes) * nmodes),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure_swapchain(&rdr);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.srf_format.format,
		    is_equal_to(VK_FORMAT_B8G8R8A8_UNORM));
	assert_that(rdr.srf_format.colorSpace,
		    is_equal_to(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR));
	assert_that(rdr.srf_mode, is_equal_to(VK_PRESENT_MODE_FIFO_KHR));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkr = create_named_test_suite("Swapchain Configuration");
	add_test(vkr, configure_selects_suitable_swapchain);
	add_test(vkr, configure_fails_when_no_memory_for_surface_capabilities);
	add_test(vkr, configure_fails_when_no_memory_for_surface_formats);
	add_test(vkr, configure_fails_when_no_surface_formats_available);
	add_test(vkr, configure_fails_when_no_suitable_surface_formats);
	add_test(vkr, configure_fails_when_no_suitable_present_modes);
	add_test(vkr, configure_selects_bgra_unorm_srgb_format_when_possible);
	add_test(vkr, configure_selects_first_when_no_bgra_unorm_srgb_format);
	add_test(vkr, configure_finds_bgra_unorm_srgb_format_in_available);
	add_test(vkr, configure_fails_when_no_memory_for_surface_modes);
	add_test(vkr, configure_fails_when_no_surface_modes_available);
	add_test(vkr, configure_selects_fifo_mode);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(vkr, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(vkr);
	return exit_code;
}
