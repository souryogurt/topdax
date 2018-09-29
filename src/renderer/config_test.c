/**
 * @file
 * Test suite for vkrenderer config selection
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <application/utils.h>
#include "vkrenderer.h"

VKAPI_ATTR VkResult VKAPI_CALL
vkEnumeratePhysicalDevices(VkInstance instance, uint32_t * pPhysicalDeviceCount,
			   VkPhysicalDevice * pPhysicalDevices)
{
	return (VkResult) mock(instance, pPhysicalDeviceCount,
			       pPhysicalDevices);
}

VKAPI_ATTR void VKAPI_CALL
vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice,
					 uint32_t * pQueueFamilyCount,
					 VkQueueFamilyProperties * pQueueFamily)
{
	mock(physicalDevice, pQueueFamilyCount, pQueueFamily);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice,
				     uint32_t queueFamilyIndex,
				     VkSurfaceKHR surface,
				     VkBool32 * pSupported)
{
	return (VkResult) mock(physicalDevice, queueFamilyIndex, surface,
			       pSupported);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice,
					  VkSurfaceKHR surface,
					  VkSurfaceCapabilitiesKHR * caps)
{
	return (VkResult) mock(physicalDevice, surface, caps);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice,
				     VkSurfaceKHR surface,
				     uint32_t * pSurfaceFormatCount,
				     VkSurfaceFormatKHR * pSurfaceFormats)
{
	return (VkResult) mock(physicalDevice, surface, pSurfaceFormatCount,
			       pSurfaceFormats);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice,
					  VkSurfaceKHR surface,
					  uint32_t * pPresentModeCount,
					  VkPresentModeKHR * pPresentModes)
{
	return (VkResult) mock(physicalDevice, surface, pPresentModeCount,
			       pPresentModes);
}

Ensure(not_enough_memory_for_devices)
{
	VkInstance instance = VK_NULL_HANDLE;
	struct vkrenderer rdr;
	expect(vkEnumeratePhysicalDevices,
	       will_return(VK_INCOMPLETE),
	       when(instance, is_equal_to(instance)));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(one_device_universal_family)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present[] = { VK_TRUE };
	VkSurfaceCapabilitiesKHR caps = { 0 };
	VkSurfaceFormatKHR fmts[1] = { 0 };
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(VkPhysicalDevice) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_set_contents_of_parameter(caps, &caps,
					      sizeof(VkSurfaceCapabilitiesKHR)),
	       when(physicalDevice, is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(VkSurfaceFormatKHR) *
					      nfmts), when(physicalDevice,
							   is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.phy, is_equal_to(phy[0]));
	assert_that(rdr.graphic, is_equal_to(0));
	assert_that(rdr.present, is_equal_to(0));
}

Ensure(one_device_separate_families)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
		{
		 .queueFlags = 0,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkSurfaceCapabilitiesKHR caps = { 0 };
	VkBool32 present[] = { VK_FALSE, VK_TRUE };
	VkSurfaceFormatKHR fmts[1] = { 0 };
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(VkPhysicalDevice) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])), when(queueFamilyIndex,
							       is_equal_to(1)),
	       will_set_contents_of_parameter(pSupported, &present[1],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_set_contents_of_parameter(caps, &caps,
					      sizeof(VkSurfaceCapabilitiesKHR)),
	       when(physicalDevice, is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(VkSurfaceFormatKHR) *
					      nfmts), when(physicalDevice,
							   is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.phy, is_equal_to(phy[0]));
	assert_that(rdr.graphic, is_equal_to(0));
	assert_that(rdr.present, is_equal_to(1));
}

Ensure(no_devices)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	uint32_t nphy = 0;
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(no_suitable_families)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
		{
		 .queueFlags = VK_QUEUE_COMPUTE_BIT,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present[] = { VK_FALSE, VK_FALSE };
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(VkPhysicalDevice) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])), when(queueFamilyIndex,
							       is_equal_to(1)),
	       will_set_contents_of_parameter(pSupported, &present[1],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(not_enough_memory_for_surface_capabilities)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present[] = { VK_TRUE };
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(VkPhysicalDevice) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));

	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_return(VK_INCOMPLETE));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(not_enough_memory_for_surface_formats)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present[] = { VK_TRUE };
	VkSurfaceCapabilitiesKHR caps = { 0 };
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(VkPhysicalDevice) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_set_contents_of_parameter(caps, &caps,
					      sizeof(VkSurfaceCapabilitiesKHR)),
	       when(physicalDevice, is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));

	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_return(VK_INCOMPLETE));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(not_enough_memory_for_surface_modes)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present[] = { VK_TRUE };
	VkSurfaceCapabilitiesKHR caps = { 0 };
	VkSurfaceFormatKHR fmts[1] = { 0 };
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(VkPhysicalDevice) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_set_contents_of_parameter(caps, &caps,
					      sizeof(VkSurfaceCapabilitiesKHR)),
	       when(physicalDevice, is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(VkSurfaceFormatKHR) *
					      nfmts), when(physicalDevice,
							   is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));

	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       will_return(VK_INCOMPLETE));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(no_surface_formats)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present[] = { VK_TRUE };
	VkSurfaceCapabilitiesKHR caps = { 0 };
	VkSurfaceFormatKHR fmts[0];
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(VkPhysicalDevice) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_set_contents_of_parameter(caps, &caps,
					      sizeof(VkSurfaceCapabilitiesKHR)),
	       when(physicalDevice, is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(VkSurfaceFormatKHR) *
					      nfmts), will_return(VK_SUCCESS));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(no_surface_modes)
{
	struct vkrenderer rdr;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present[] = { VK_TRUE };
	VkSurfaceCapabilitiesKHR caps = { 0 };
	VkSurfaceFormatKHR fmts[1];
	uint32_t nfmts = ARRAY_SIZE(fmts);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(VkPhysicalDevice) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(phy[0])),
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
	       will_set_contents_of_parameter(caps, &caps,
					      sizeof(VkSurfaceCapabilitiesKHR)),
	       when(physicalDevice, is_equal_to(phy[0])),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceFormatsKHR,
	       will_set_contents_of_parameter(pSurfaceFormatCount, &nfmts,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pSurfaceFormats, &fmts,
					      sizeof(VkSurfaceFormatKHR) *
					      nfmts), will_return(VK_SUCCESS));
	uint32_t nmodes = 0;
	expect(vkGetPhysicalDeviceSurfacePresentModesKHR,
	       will_set_contents_of_parameter(pPresentModeCount, &nmodes,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkr = create_named_test_suite("VKRenderer Configuration");
	add_test(vkr, not_enough_memory_for_devices);
	add_test(vkr, one_device_universal_family);
	add_test(vkr, one_device_separate_families);
	add_test(vkr, no_devices);
	add_test(vkr, no_suitable_families);
	add_test(vkr, not_enough_memory_for_surface_capabilities);
	add_test(vkr, not_enough_memory_for_surface_formats);
	add_test(vkr, not_enough_memory_for_surface_modes);
	add_test(vkr, no_surface_formats);
	add_test(vkr, no_surface_modes);
	return run_test_suite(vkr, create_text_reporter());
}
