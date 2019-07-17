/**
 * @file
 * Test suite for vkrenderer config selection
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <vulkan/vulkan_core.h>
#include "vkrenderer.h"
#include "phy_device.h"

VKAPI_ATTR VkResult VKAPI_CALL
vkEnumeratePhysicalDevices(VkInstance instance, uint32_t *pPhysicalDeviceCount,
			   VkPhysicalDevice *pPhysicalDevices)
{
	return (VkResult)mock(instance, pPhysicalDeviceCount, pPhysicalDevices);
}

void phy_device_init(struct phy_device *dev, VkPhysicalDevice phy)
{
	mock(dev, phy);
}

bool phy_find_graphic_family(const struct phy_device *dev, uint32_t *index)
{
	return (bool)mock(dev, index);
}

bool phy_find_present_family(const struct phy_device *dev, VkSurfaceKHR srf,
			     uint32_t *index)
{
	return (bool)mock(dev, srf, index);
}

bool phy_find_universal_family(const struct phy_device *dev, VkSurfaceKHR srf,
			       uint32_t *index)
{
	return (bool)mock(dev, srf, index);
}

int vkrenderer_configure_swapchain(struct vkrenderer *rdr)
{
	return (int)mock(rdr);
}

Ensure(configure_fails_when_not_enough_memory_for_devices_list)
{
	VkInstance instance = VK_NULL_HANDLE;
	struct vkrenderer rdr = { 0 };
	expect(vkEnumeratePhysicalDevices, will_return(VK_INCOMPLETE),
	       when(instance, is_equal_to(instance)));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_fails_when_no_devices_available)
{
	struct vkrenderer rdr = { 0 };
	VkInstance instance = VK_NULL_HANDLE;
	uint32_t nphy = 0;
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(nphy)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_selects_suitable_device)
{
	struct vkrenderer rdr = { 0 };
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1] = { VK_NULL_HANDLE };
	uint32_t nphy = ARRAY_SIZE(phy);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(*phy) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(nphy)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(phy_device_init);
	expect(phy_find_universal_family, will_return(true));
	expect(vkrenderer_configure_swapchain, will_return(0));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_equal_to(0));
	assert_that(rdr.phy, is_equal_to(phy[0]));
}

Ensure(configure_fails_when_no_suitable_families_available)
{
	struct vkrenderer rdr = { 0 };
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1] = { VK_NULL_HANDLE };
	uint32_t nphy = ARRAY_SIZE(phy);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(*phy) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(nphy)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(phy_device_init);
	expect(phy_find_universal_family, will_return(false));
	expect(phy_find_graphic_family, will_return(false));
	never_expect(vkrenderer_configure_swapchain, will_return(0));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

Ensure(configure_fails_when_no_suitable_swapchain_available)
{
	struct vkrenderer rdr = { 0 };
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice phy[1] = { VK_NULL_HANDLE };
	uint32_t nphy = ARRAY_SIZE(phy);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(*phy) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(nphy)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(phy_device_init);
	expect(phy_find_universal_family, will_return(false));
	expect(phy_find_graphic_family, will_return(true));
	expect(phy_find_present_family, will_return(true));
	expect(vkrenderer_configure_swapchain, will_return(-1));
	int result = vkrenderer_configure(&rdr, instance);
	assert_that(result, is_not_equal_to(0));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkr = create_named_test_suite("Device configuration");
	add_test(vkr, configure_fails_when_not_enough_memory_for_devices_list);
	add_test(vkr, configure_fails_when_no_devices_available);
	add_test(vkr, configure_selects_suitable_device);
	add_test(vkr, configure_fails_when_no_suitable_families_available);
	add_test(vkr, configure_fails_when_no_suitable_swapchain_available);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(vkr, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(vkr);
	return exit_code;
}
