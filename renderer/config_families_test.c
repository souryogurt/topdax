/**
 * @file
 * Test suite for vkrenderer families selection
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <vulkan/vulkan_core.h>
#include "vkrenderer.h"

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

Ensure(configure_selects_universal_queue_family)
{
	struct vkrenderer rdr;
	VkQueueFamilyProperties fams[] = {
		{
		 .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		 .queueCount = 1,
		 },
	};
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present[] = { VK_TRUE };
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure_families(&rdr);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.graphic, is_equal_to(0));
	assert_that(rdr.present, is_equal_to(0));
}

Ensure(configure_selects_separate_families_when_no_universal)
{
	struct vkrenderer rdr;
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
	VkBool32 present[] = { VK_FALSE, VK_TRUE };
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(1)),
	       will_set_contents_of_parameter(pSupported, &present[1],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure_families(&rdr);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.graphic, is_equal_to(0));
	assert_that(rdr.present, is_equal_to(1));
}

Ensure(configure_fails_when_no_suitable_families)
{
	struct vkrenderer rdr;
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
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       will_set_contents_of_parameter(pQueueFamilyCount, &nfams,
					      sizeof(uint32_t)),
	       will_set_contents_of_parameter(pQueueFamily, fams,
					      sizeof(VkQueueFamilyProperties) *
					      nfams)
	    );
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(1)),
	       will_set_contents_of_parameter(pSupported, &present[1],
					      sizeof(VkBool32)),
	       will_return(VK_SUCCESS));
	int result = vkrenderer_configure_families(&rdr);
	assert_that(result, is_not_equal_to(0));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkr = create_named_test_suite("Families configuration");
	add_test(vkr, configure_selects_universal_queue_family);
	add_test(vkr, configure_selects_separate_families_when_no_universal);
	add_test(vkr, configure_fails_when_no_suitable_families);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(vkr, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(vkr);
	return exit_code;
}
