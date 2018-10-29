/**
 * @file
 * Test suite for vkrenderer
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <renderer/vkrenderer.h>
#include "vkrenderer.h"

int vkrenderer_configure(struct vkrenderer *rdr, VkInstance instance)
{
	return (int)mock(rdr, instance);
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

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateCommandPool(VkDevice device,
		    const VkCommandPoolCreateInfo * pCreateInfo,
		    const VkAllocationCallbacks * pAllocator,
		    VkCommandPool * pCommandPool)
{
	return (VkResult) mock(device, pCreateInfo, pAllocator, pCommandPool);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool,
		     const VkAllocationCallbacks * pAllocator)
{
	mock(device, commandPool, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vkDeviceWaitIdle(VkDevice device)
{
	return (VkResult) mock(device);
}

int vkswapchain_init(struct vkswapchain *swc, const struct vkrenderer *rdr)
{
	return (int)mock(swc, rdr);
}

void vkswapchain_terminate(const struct vkswapchain *swc, VkDevice dev)
{
	mock(swc, dev);
}

VkResult vkswapchain_render(const struct vkswapchain *swc,
			    const struct vkrenderer *rdr)
{
	return (VkResult) mock(swc, rdr);
}

Ensure(init_returns_zero_on_success)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateCommandPool, will_return(VK_SUCCESS));
	expect(vkswapchain_init, will_return(0));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_equal_to(0));
}

Ensure(init_returns_non_zero_when_no_configs)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(1));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_device_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_ERROR_INITIALIZATION_FAILED));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_command_pool_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateCommandPool, will_return(VK_NOT_READY));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_swapchain_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateCommandPool, will_return(VK_SUCCESS));
	expect(vkswapchain_init, will_return(-1));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(render_returns_zero_on_success)
{
	struct vkrenderer vkr = { 0 };
	expect(vkswapchain_render,
	       when(swc, is_equal_to(&vkr.swcs[0])),
	       when(rdr, is_equal_to(&vkr)), will_return(VK_SUCCESS));
	int error = vkrenderer_render(&vkr);
	assert_that(error, is_equal_to(0));
}

Ensure(render_recreates_swapchain)
{
	struct vkrenderer vkr = { 0 };
	expect(vkswapchain_render,
	       when(swc, is_equal_to(&vkr.swcs[0])),
	       when(rdr, is_equal_to(&vkr)),
	       will_return(VK_ERROR_OUT_OF_DATE_KHR));
	expect(vkswapchain_init, when(swc, is_equal_to(&vkr.swcs[1])));
	expect(vkswapchain_render,
	       when(swc, is_equal_to(&vkr.swcs[1])),
	       when(rdr, is_equal_to(&vkr)), will_return(VK_SUCCESS));
	int error = vkrenderer_render(&vkr);
	assert_that(error, is_equal_to(0));
}

Ensure(terminate_destroys_all_resources)
{
	struct vkrenderer vkr = { 0 };
	expect(vkDeviceWaitIdle);
	expect(vkswapchain_terminate);
	expect(vkDestroyCommandPool);
	expect(vkDestroyDevice);

	vkrenderer_terminate(&vkr);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkr = create_named_test_suite("VKRenderer");
	add_test(vkr, init_returns_zero_on_success);
	add_test(vkr, init_returns_non_zero_when_no_configs);
	add_test(vkr, init_returns_non_zero_on_device_fail);
	add_test(vkr, init_returns_non_zero_on_command_pool_fail);
	add_test(vkr, init_returns_non_zero_on_swapchain_fail);
	add_test(vkr, render_returns_zero_on_success);
	add_test(vkr, render_recreates_swapchain);
	add_test(vkr, terminate_destroys_all_resources);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(vkr, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(vkr);
	return exit_code;
}
