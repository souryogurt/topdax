/**
 * @file
 * Test suite for phy_device
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <vulkan/vulkan_core.h>
#include "phy_device.h"

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(
	VkPhysicalDevice physicalDevice, uint32_t *pQueueFamilyCount,
	VkQueueFamilyProperties *pQueueFamily)
{
	mock(physicalDevice, pQueueFamilyCount, pQueueFamily);
}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(
	VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex,
	VkSurfaceKHR surface, VkBool32 *pSupported)
{
	return (VkResult)mock(physicalDevice, queueFamilyIndex, surface,
			      pSupported);
}

Ensure(phy_device_init_fills_properties)
{
	VkQueueFamilyProperties fams[3];
	struct phy_device dev = {
		.nfams = 3,
		.fams = fams,
	};
	VkPhysicalDevice phy = (VkPhysicalDevice)!VK_NULL_HANDLE;
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       when(physicalDevice, is_equal_to(phy)),
	       when(pQueueFamilyCount, is_equal_to(&dev.nfams)),
	       when(pQueueFamily, is_equal_to(dev.fams)));
	phy_device_init(&dev, phy);
	assert_that(dev.phy, is_equal_to(phy));
}

Ensure(phy_find_graphic_family_returns_true_on_success)
{
	VkQueueFamilyProperties fams[] = {
		{
			.queueFlags = 0,
		},
		{
			.queueFlags = VK_QUEUE_GRAPHICS_BIT,
		},

	};
	struct phy_device dev = {
		.nfams = 2,
		.fams = fams,
	};
	uint32_t index = 0;
	bool found = phy_find_graphic_family(&dev, &index);
	assert_that(found, is_true);
	assert_that(index, is_equal_to(1));
}

Ensure(phy_find_graphic_family_returns_false_on_fail)
{
	VkQueueFamilyProperties fams[] = {
		{
			.queueFlags = 0,
		},
		{
			.queueFlags = VK_QUEUE_COMPUTE_BIT,
		},

	};
	struct phy_device dev = {
		.nfams = 2,
		.fams = fams,
	};
	uint32_t index = 0;
	bool found = phy_find_graphic_family(&dev, &index);
	assert_that(found, is_false);
}

Ensure(phy_find_present_family_returns_true_on_success)
{
	struct phy_device dev = {
		.nfams = 2,
		.phy = (VkPhysicalDevice)!VK_NULL_HANDLE,
	};
	VkSurfaceKHR srf = (VkSurfaceKHR)!VK_NULL_HANDLE;
	uint32_t index = 0;

	VkBool32 present[] = { VK_FALSE, VK_TRUE };
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(*present)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(1)),
	       will_set_contents_of_parameter(pSupported, &present[1],
					      sizeof(*present)),
	       will_return(VK_SUCCESS));
	bool found = phy_find_present_family(&dev, srf, &index);
	assert_that(found, is_true);
	assert_that(index, is_equal_to(1));
}

Ensure(phy_find_present_family_returns_false_on_fail)
{
	struct phy_device dev = {
		.nfams = 2,
		.phy = (VkPhysicalDevice)!VK_NULL_HANDLE,
	};
	VkSurfaceKHR srf = (VkSurfaceKHR)!VK_NULL_HANDLE;
	uint32_t index = 0;

	VkBool32 present[] = { VK_FALSE, VK_FALSE };
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(*present)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(1)),
	       will_set_contents_of_parameter(pSupported, &present[1],
					      sizeof(*present)),
	       will_return(VK_SUCCESS));
	bool found = phy_find_present_family(&dev, srf, &index);
	assert_that(found, is_false);
}

Ensure(phy_find_universal_family_returns_true_on_success)
{
	VkQueueFamilyProperties fams[] = {
		{
			.queueFlags = 0,
		},
		{
			.queueFlags = VK_QUEUE_GRAPHICS_BIT,
		},

	};
	VkBool32 present[] = { VK_FALSE, VK_TRUE };
	struct phy_device dev = {
		.nfams = 2,
		.fams = fams,
		.phy = (VkPhysicalDevice)!VK_NULL_HANDLE,
	};
	VkSurfaceKHR srf = (VkSurfaceKHR)!VK_NULL_HANDLE;
	uint32_t index = 0;

	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(*present)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(1)),
	       will_set_contents_of_parameter(pSupported, &present[1],
					      sizeof(*present)),
	       will_return(VK_SUCCESS));
	bool found = phy_find_universal_family(&dev, srf, &index);
	assert_that(found, is_true);
	assert_that(index, is_equal_to(1));
}

Ensure(phy_find_universal_family_returns_false_on_fail)
{
	VkQueueFamilyProperties fams[] = {
		{
			.queueFlags = 0,
		},
		{
			.queueFlags = VK_QUEUE_GRAPHICS_BIT,
		},

	};
	VkBool32 present[] = { VK_FALSE, VK_FALSE };
	struct phy_device dev = {
		.nfams = 2,
		.fams = fams,
		.phy = (VkPhysicalDevice)!VK_NULL_HANDLE,
	};
	VkSurfaceKHR srf = (VkSurfaceKHR)!VK_NULL_HANDLE;
	uint32_t index = 0;

	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present[0],
					      sizeof(*present)),
	       will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(1)),
	       will_set_contents_of_parameter(pSupported, &present[1],
					      sizeof(*present)),
	       will_return(VK_SUCCESS));
	bool found = phy_find_universal_family(&dev, srf, &index);
	assert_that(found, is_false);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *phy = create_named_test_suite("phy_device");
	add_test(phy, phy_device_init_fills_properties);
	add_test(phy, phy_find_graphic_family_returns_true_on_success);
	add_test(phy, phy_find_graphic_family_returns_false_on_fail);
	add_test(phy, phy_find_present_family_returns_true_on_success);
	add_test(phy, phy_find_present_family_returns_false_on_fail);
	add_test(phy, phy_find_universal_family_returns_true_on_success);
	add_test(phy, phy_find_universal_family_returns_false_on_fail);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(phy, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(phy);
	return exit_code;
}
