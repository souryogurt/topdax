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

Ensure(phy_family_count_returns_number_of_families)
{
	struct phy_device dev = {
		.nfams = 3,
	};
	assert_that(phy_family_count(&dev), is_equal_to(3));
}

Ensure(phy_family_can_graphics_returns_support_of_graphics_ops)
{
	VkQueueFamilyProperties props;
	struct phy_device dev = {
		.nfams = 1,
		.fams = &props,
	};

	props.queueFlags = VK_QUEUE_GRAPHICS_BIT;
	assert_that(phy_family_can_graphics(&dev, 0), is_true);

	props.queueFlags = VK_QUEUE_COMPUTE_BIT;
	assert_that(phy_family_can_graphics(&dev, 0), is_false);
}

Ensure(phy_family_can_present_returns_support_of_present_ops)
{
	VkBool32 present;
	VkSurfaceKHR srf = (VkSurfaceKHR)!VK_NULL_HANDLE;
	struct phy_device dev = {
		.phy = (VkPhysicalDevice)!VK_NULL_HANDLE,
	};

	present = VK_TRUE;
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present,
					      sizeof(present)),
	       will_return(VK_SUCCESS));
	assert_that(phy_family_can_present(&dev, 0, srf), is_true);

	present = VK_FALSE;
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(queueFamilyIndex, is_equal_to(0)),
	       will_set_contents_of_parameter(pSupported, &present,
					      sizeof(present)),
	       will_return(VK_SUCCESS));
	assert_that(phy_family_can_present(&dev, 0, srf), is_false);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *phy = create_named_test_suite("phy_device");
	add_test(phy, phy_device_init_fills_properties);
	add_test(phy, phy_family_count_returns_number_of_families);
	add_test(phy, phy_family_can_graphics_returns_support_of_graphics_ops);
	add_test(phy, phy_family_can_present_returns_support_of_present_ops);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(phy, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(phy);
	return exit_code;
}
