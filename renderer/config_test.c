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
#include "config_swapchain.h"
#include "config.h"
#include "phy_device.h"

Ensure(configure_fails_when_not_enough_memory_for_devices_list)
{
	VkInstance instance = (VkInstance)!VK_NULL_HANDLE;
	struct vkrenderer rdr = { 0 };
	expect(vkEnumeratePhysicalDevices, will_return(VK_INCOMPLETE),
	       when(instance, is_equal_to(instance)));

	assert_that(vkrenderer_configure(&rdr, instance), is_not_equal_to(0));
}

Ensure(configure_fails_when_no_devices_available)
{
	struct vkrenderer rdr = { 0 };
	VkInstance instance = (VkInstance)!VK_NULL_HANDLE;
	uint32_t nphy = 0;
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(nphy)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	assert_that(vkrenderer_configure(&rdr, instance), is_not_equal_to(0));
}

Ensure(configure_selects_suitable_device_with_universal_queue)
{
	struct vkrenderer rdr = { 0 };
	VkInstance instance = (VkInstance)!VK_NULL_HANDLE;
	VkPhysicalDevice phy[1] = { (VkPhysicalDevice)!VK_NULL_HANDLE };
	uint32_t nphy = ARRAY_SIZE(phy);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(*phy) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(nphy)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(phy_device_init);
	expect(phy_family_count, will_return(1));
	expect(phy_family_can_graphics, will_return(true));
	expect(phy_family_can_present, will_return(true));

	assert_that(vkrenderer_configure(&rdr, instance), is_equal_to(0));
	assert_that(rdr.phy, is_equal_to(phy[0]));
	assert_that(rdr.graphic, is_equal_to(0));
	assert_that(rdr.present, is_equal_to(0));
}

Ensure(configure_selects_suitable_device_with_separate_queues)
{
	struct vkrenderer rdr = { 0 };
	VkInstance instance = (VkInstance)!VK_NULL_HANDLE;
	VkPhysicalDevice phy[1] = { (VkPhysicalDevice)!VK_NULL_HANDLE };
	uint32_t nphy = ARRAY_SIZE(phy);
	expect(vkEnumeratePhysicalDevices,
	       will_set_contents_of_parameter(pPhysicalDevices, phy,
					      sizeof(*phy) * nphy),
	       will_set_contents_of_parameter(pPhysicalDeviceCount, &nphy,
					      sizeof(nphy)),
	       will_return(VK_SUCCESS), when(instance, is_equal_to(instance)));
	expect(phy_device_init);
	expect(phy_family_count, will_return(2));
	expect(phy_family_can_graphics, will_return(true));
	expect(phy_family_can_present, will_return(false));
	expect(phy_family_can_graphics, will_return(false));

	expect(phy_family_can_graphics, will_return(true));
	expect(phy_family_can_present, will_return(false));
	expect(phy_family_can_present, will_return(true));

	assert_that(vkrenderer_configure(&rdr, instance), is_equal_to(0));
	assert_that(rdr.phy, is_equal_to(phy[0]));
	assert_that(rdr.graphic, is_equal_to(0));
	assert_that(rdr.present, is_equal_to(1));
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
	expect(phy_family_count, will_return(1));
	expect(phy_family_can_graphics, will_return(true));
	expect(phy_family_can_present, will_return(false));
	expect(phy_family_can_graphics, will_return(true));
	expect(phy_family_can_present, will_return(false));

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
	add_test(vkr, configure_selects_suitable_device_with_universal_queue);
	add_test(vkr, configure_selects_suitable_device_with_separate_queues);
	add_test(vkr, configure_fails_when_no_suitable_families_available);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(vkr, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(vkr);
	return exit_code;
}
