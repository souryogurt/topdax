/**
 * @file
 * Test suite for vkrenderer
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <renderer/vkrenderer.h>
#include "vkrenderer.h"

int choose_config(VkInstance instance, struct device_config *cfg,
		  VkSurfaceKHR surface)
{
	return (int)mock(instance, cfg, surface);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice,
					      const VkDeviceCreateInfo *
					      pCreateInfo,
					      const VkAllocationCallbacks *
					      pAllocator, VkDevice * pDevice)
{
	return (VkResult) mock(physicalDevice, pCreateInfo, pAllocator,
			       pDevice);
}

VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue(VkDevice device,
					    uint32_t queueFamilyIndex,
					    uint32_t queueIndex,
					    VkQueue * pQueue)
{
	mock(device, queueFamilyIndex, queueIndex, pQueue);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDevice(VkDevice device,
					   const VkAllocationCallbacks *
					   pAllocator)
{
	mock(device, pAllocator);
}

Ensure(vkrenderer_init_returns_zero_on_success)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(choose_config, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_when_no_configs)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(choose_config, will_return(1));
	never_expect(vkCreateDevice, will_return(VK_SUCCESS));
	never_expect(vkGetDeviceQueue);
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_on_device_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(choose_config, will_return(0));
	expect(vkCreateDevice, will_return(VK_ERROR_INITIALIZATION_FAILED));
	never_expect(vkGetDeviceQueue);
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_terminate_destroys_all_resources)
{
	struct vkrenderer vkr;
	expect(vkDestroyDevice);
	vkrenderer_terminate(&vkr);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkr = create_named_test_suite("VKRenderer");
	add_test(vkr, vkrenderer_init_returns_zero_on_success);
	add_test(vkr, vkrenderer_init_returns_non_zero_when_no_configs);
	add_test(vkr, vkrenderer_init_returns_non_zero_on_device_fail);
	add_test(vkr, vkrenderer_terminate_destroys_all_resources);
	return run_test_suite(vkr, create_text_reporter());
}
