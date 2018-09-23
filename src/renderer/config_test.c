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

struct test_device_config {
	VkQueueFamilyProperties fams[32];
	VkBool32 present_support[32];
	uint32_t nfams;
};

struct test_device_config device_configs[] = {
	/* Device with one universal family queue */
	{
	 .fams = {
		  {
		   .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		   .queueCount = 1,
		   .timestampValidBits = 0,
		   .minImageTransferGranularity = {

						   .width = 0,
						   .height = 0,
						   .depth = 0,
						   },
		   },
		  },
	 .present_support = {VK_TRUE},
	 .nfams = 1,
	 },
	/* Device with two different family queues */
	{
	 .fams = {
		  {
		   .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		   .queueCount = 1,
		   .timestampValidBits = 0,
		   .minImageTransferGranularity = {

						   .width = 0,
						   .height = 0,
						   .depth = 0,
						   },
		   },
		  {
		   .queueFlags = 0,
		   .queueCount = 1,
		   .timestampValidBits = 0,
		   .minImageTransferGranularity = {

						   .width = 0,
						   .height = 0,
						   .depth = 0,
						   },
		   },
		  },
	 .present_support = {VK_FALSE, VK_TRUE},
	 .nfams = 2,
	 },
	/* Device with two unsuitable family queues */
	{
	 .fams = {
		  {
		   .queueFlags = VK_QUEUE_GRAPHICS_BIT,
		   .queueCount = 1,
		   .timestampValidBits = 0,
		   .minImageTransferGranularity = {

						   .width = 0,
						   .height = 0,
						   .depth = 0,
						   },
		   },
		  {
		   .queueFlags = VK_QUEUE_COMPUTE_BIT,
		   .queueCount = 1,
		   .timestampValidBits = 0,
		   .minImageTransferGranularity = {

						   .width = 0,
						   .height = 0,
						   .depth = 0,
						   },
		   },
		  },
	 .present_support = {VK_FALSE, VK_FALSE},
	 .nfams = 2,
	 },
};

struct test_instance_config {
	VkPhysicalDevice phy[4];
	uint32_t nphy;
};

struct test_instance_config instance_configs[] = {
	/* One device configuration */
	{
	 .phy = {(VkPhysicalDevice) 0},
	 .nphy = 1,
	 },
	/* One device with two families */
	{
	 .phy = {(VkPhysicalDevice) 1},
	 .nphy = 1,
	 },
	/* No devices */
	{
	 .nphy = 0,
	 },
	/* One device with two unsuitable families */
	{
	 .phy = {(VkPhysicalDevice) 2},
	 .nphy = 1,
	 },
};

VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDevices(VkInstance
							  instance,
							  uint32_t *
							  pPhysicalDeviceCount,
							  VkPhysicalDevice
							  * pPhysicalDevices)
{
	mock(instance, pPhysicalDeviceCount, pPhysicalDevices);
	size_t test_i = (size_t) instance;
	struct test_instance_config *cfg = &instance_configs[test_i];
	*pPhysicalDeviceCount = cfg->nphy;
	memcpy(pPhysicalDevices, cfg->phy,
	       sizeof(VkPhysicalDevice) * cfg->nphy);
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL
vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice,
					 uint32_t * pQueueFamilyCount,
					 VkQueueFamilyProperties * pQueueFamily)
{
	mock(physicalDevice, pQueueFamilyCount, pQueueFamily);
	size_t test_i = (size_t) physicalDevice;
	struct test_device_config *cfg = &device_configs[test_i];
	*pQueueFamilyCount = cfg->nfams;
	memcpy(pQueueFamily, cfg->fams,
	       sizeof(VkQueueFamilyProperties) * cfg->nfams);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice,
				     uint32_t queueFamilyIndex,
				     VkSurfaceKHR surface,
				     VkBool32 * pSupported)
{
	VkResult result = (VkResult) mock(physicalDevice, queueFamilyIndex,
					  surface, pSupported);
	size_t test_i = (size_t) physicalDevice;
	struct test_device_config *cfg = &device_configs[test_i];
	*pSupported = cfg->present_support[queueFamilyIndex];
	return result;
}

Ensure(one_device_universal_family)
{
	VkInstance instance = (VkInstance) 0;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct device_config cfg;
	expect(vkEnumeratePhysicalDevices,
	       when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       when(physicalDevice, is_equal_to(0)));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(0)), will_return(VK_SUCCESS));
	int result = choose_config(instance, &cfg, surface);
	assert_that(result, is_equal_to(0));
	assert_that(cfg.phy, is_equal_to(0));
	assert_that(cfg.graph_family, is_equal_to(0));
	assert_that(cfg.present_family, is_equal_to(0));
}

Ensure(one_device_separate_families)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct device_config cfg;
	expect(vkEnumeratePhysicalDevices,
	       when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       when(physicalDevice, is_equal_to(1)));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(1)), will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(1)), will_return(VK_SUCCESS));
	int result = choose_config(instance, &cfg, surface);
	assert_that(result, is_equal_to(0));
	assert_that(cfg.phy, is_equal_to(1));
	assert_that(cfg.graph_family, is_equal_to(0));
	assert_that(cfg.present_family, is_equal_to(1));
}

Ensure(no_devices)
{
	VkInstance instance = (VkInstance) 2;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct device_config cfg;
	expect(vkEnumeratePhysicalDevices,
	       when(instance, is_equal_to(instance)));
	never_expect(vkGetPhysicalDeviceQueueFamilyProperties);
	never_expect(vkGetPhysicalDeviceSurfaceSupportKHR);
	int result = choose_config(instance, &cfg, surface);
	assert_that(result, is_not_equal_to(0));
}

Ensure(no_suitable_families)
{
	VkInstance instance = (VkInstance) 3;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct device_config cfg;
	expect(vkEnumeratePhysicalDevices,
	       when(instance, is_equal_to(instance)));
	expect(vkGetPhysicalDeviceQueueFamilyProperties,
	       when(physicalDevice, is_equal_to(2)));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(2)), will_return(VK_SUCCESS));
	expect(vkGetPhysicalDeviceSurfaceSupportKHR,
	       when(physicalDevice, is_equal_to(2)), will_return(VK_SUCCESS));
	int result = choose_config(instance, &cfg, surface);
	assert_that(result, is_not_equal_to(0));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkr = create_named_test_suite("VKRenderer Configuration");
	add_test(vkr, one_device_universal_family);
	add_test(vkr, one_device_separate_families);
	add_test(vkr, no_devices);
	add_test(vkr, no_suitable_families);
	return run_test_suite(vkr, create_text_reporter());
}
