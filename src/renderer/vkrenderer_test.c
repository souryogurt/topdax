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

VKAPI_ATTR VkResult VKAPI_CALL
vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain,
		      uint64_t timeout, VkSemaphore semaphore, VkFence fence,
		      uint32_t * pImageIndex)
{
	return (VkResult) mock(device, swapchain, timeout, semaphore, fence,
			       pImageIndex);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkQueueSubmit(VkQueue queue, uint32_t submitCount,
	      const VkSubmitInfo * pSubmits, VkFence fence)
{
	return (VkResult) mock(queue, submitCount, pSubmits, fence);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR * pPresentInfo)
{
	return (VkResult) mock(queue, pPresentInfo);
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

Ensure(vkrenderer_init_returns_zero_on_success)
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

Ensure(vkrenderer_init_returns_non_zero_when_no_configs)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(1));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_on_device_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_ERROR_INITIALIZATION_FAILED));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_on_command_pool_fail)
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

Ensure(vkrenderer_init_returns_non_zero_on_swapchain_fail)
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

Ensure(vkswapchain_render_returns_error_on_image_acquire_fail)
{
	struct vkrenderer vkr = { 0 };
	expect(vkAcquireNextImageKHR, will_return(VK_NOT_READY));
	VkResult error = vkswapchain_render(&vkr.swapchain, &vkr);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkswapchain_render_returns_error_on_submit_fail)
{
	struct vkrenderer vkr = { 0 };
	uint32_t image_index = 0;
	expect(vkAcquireNextImageKHR,
	       will_set_contents_of_parameter(pImageIndex, &image_index,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	expect(vkQueueSubmit, will_return(VK_NOT_READY));
	VkResult error = vkswapchain_render(&vkr.swapchain, &vkr);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkswapchain_render_returns_error_on_present_fail)
{
	struct vkrenderer vkr = { 0 };
	uint32_t image_index = 0;
	expect(vkAcquireNextImageKHR,
	       will_set_contents_of_parameter(pImageIndex, &image_index,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	expect(vkQueueSubmit, will_return(VK_SUCCESS));
	expect(vkQueuePresentKHR, will_return(VK_NOT_READY));
	VkResult error = vkswapchain_render(&vkr.swapchain, &vkr);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkrenderer_terminate_destroys_all_resources)
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
	add_test(vkr, vkrenderer_init_returns_zero_on_success);
	add_test(vkr, vkrenderer_init_returns_non_zero_when_no_configs);
	add_test(vkr, vkrenderer_init_returns_non_zero_on_device_fail);
	add_test(vkr, vkrenderer_init_returns_non_zero_on_command_pool_fail);
	add_test(vkr, vkrenderer_init_returns_non_zero_on_swapchain_fail);
	add_test(vkr, vkswapchain_render_returns_error_on_image_acquire_fail);
	add_test(vkr, vkswapchain_render_returns_error_on_submit_fail);
	add_test(vkr, vkswapchain_render_returns_error_on_present_fail);
	add_test(vkr, vkrenderer_terminate_destroys_all_resources);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(vkr, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(vkr);
	return exit_code;
}
