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
#include <renderer/vkswapchain.h>

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateSwapchainKHR(VkDevice device,
		     const VkSwapchainCreateInfoKHR * pCreateInfo,
		     const VkAllocationCallbacks * pAllocator,
		     VkSwapchainKHR * pSwapchain)
{
	return (VkResult) mock(device, pCreateInfo, pAllocator, pSwapchain);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain,
		      const VkAllocationCallbacks * pAllocator)
{
	mock(device, swapchain, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain,
			uint32_t * pSwapchainImageCount,
			VkImage * pSwapchainImages)
{
	return (VkResult) mock(device, swapchain, pSwapchainImageCount,
			       pSwapchainImages);
}

VkResult
vkframe_init(struct vkframe *frame, const VkRenderPass rpass,
	     const struct vkrenderer *rdr, const VkImage image)
{
	return (VkResult) mock(frame, rpass, rdr, image);
}

void vkframe_destroy(const struct vkframe *frame, const VkDevice device)
{
	mock(frame, device);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo * pCreateInfo,
		  const VkAllocationCallbacks * pAllocator,
		  VkSemaphore * pSemaphore)
{
	return (VkResult) mock(device, pCreateInfo, pAllocator, pSemaphore);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroySemaphore(VkDevice device, VkSemaphore semaphore,
		   const VkAllocationCallbacks * pAllocator)
{
	mock(device, semaphore, pAllocator);
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

Ensure(init_returns_zero_on_success)
{
	struct vkrenderer vkr = { 0 };
	struct vkswapchain swc = { 0 };
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	uint32_t nimgs = 1;
	expect(vkGetSwapchainImagesKHR,
	       will_set_contents_of_parameter(pSwapchainImageCount, &nimgs,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	expect(vkCreateSemaphore, will_return(VK_SUCCESS));
	expect(vkCreateSemaphore, will_return(VK_SUCCESS));
	expect(vkframe_init, will_return(VK_SUCCESS));
	int error = vkswapchain_init(&swc, &vkr, VK_NULL_HANDLE);
	assert_that(error, is_equal_to(0));
}

Ensure(init_returns_non_zero_on_swapchain_fail)
{
	struct vkrenderer vkr = { 0 };
	struct vkswapchain swc = { 0 };
	expect(vkCreateSwapchainKHR, will_return(VK_NOT_READY));
	int error = vkswapchain_init(&swc, &vkr, VK_NULL_HANDLE);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_sync_objects_fail)
{
	struct vkrenderer vkr = { 0 };
	struct vkswapchain swc = { 0 };
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	expect(vkCreateSemaphore, will_return(VK_NOT_READY));
	int error = vkswapchain_init(&swc, &vkr, VK_NULL_HANDLE);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_getting_images_fail)
{
	struct vkrenderer vkr = { 0 };
	struct vkswapchain swc = { 0 };
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	expect(vkCreateSemaphore, will_return(VK_SUCCESS));
	expect(vkCreateSemaphore, will_return(VK_SUCCESS));
	expect(vkGetSwapchainImagesKHR, will_return(VK_INCOMPLETE));
	int error = vkswapchain_init(&swc, &vkr, VK_NULL_HANDLE);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_frame_init_fail)
{
	struct vkrenderer vkr = { 0 };
	struct vkswapchain swc = { 0 };
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	expect(vkCreateSemaphore, will_return(VK_SUCCESS));
	expect(vkCreateSemaphore, will_return(VK_SUCCESS));
	uint32_t nimgs = 1;
	expect(vkGetSwapchainImagesKHR,
	       will_set_contents_of_parameter(pSwapchainImageCount, &nimgs,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	expect(vkframe_init, will_return(VK_ERROR_INITIALIZATION_FAILED));
	int error = vkswapchain_init(&swc, &vkr, VK_NULL_HANDLE);
	assert_that(error, is_not_equal_to(0));
}

Ensure(terminate_destroys_all_resources)
{
	VkDevice dev = VK_NULL_HANDLE;
	struct vkswapchain swc = {
		.nframes = 1,
	};
	expect(vkframe_destroy);
	expect(vkDestroySemaphore);
	expect(vkDestroySemaphore);
	expect(vkDestroySwapchainKHR);
	vkswapchain_terminate(&swc, dev);
}

Ensure(render_returns_error_on_image_acquire_fail)
{
	struct vkrenderer vkr = { 0 };
	expect(vkAcquireNextImageKHR, will_return(VK_NOT_READY));
	VkResult error = vkswapchain_render(&vkr.swcs[0], &vkr);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(render_returns_error_on_submit_fail)
{
	struct vkrenderer vkr = { 0 };
	uint32_t image_index = 0;
	expect(vkAcquireNextImageKHR,
	       will_set_contents_of_parameter(pImageIndex, &image_index,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	expect(vkQueueSubmit, will_return(VK_NOT_READY));
	VkResult error = vkswapchain_render(&vkr.swcs[0], &vkr);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(render_returns_error_on_present_fail)
{
	struct vkrenderer vkr = { 0 };
	uint32_t image_index = 0;
	expect(vkAcquireNextImageKHR,
	       will_set_contents_of_parameter(pImageIndex, &image_index,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	expect(vkQueueSubmit, will_return(VK_SUCCESS));
	expect(vkQueuePresentKHR, will_return(VK_NOT_READY));
	VkResult error = vkswapchain_render(&vkr.swcs[0], &vkr);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *swc = create_named_test_suite("VKSwapchain");
	add_test(swc, init_returns_zero_on_success);
	add_test(swc, init_returns_non_zero_on_swapchain_fail);
	add_test(swc, init_returns_non_zero_on_sync_objects_fail);
	add_test(swc, init_returns_non_zero_on_getting_images_fail);
	add_test(swc, init_returns_non_zero_on_frame_init_fail);
	add_test(swc, render_returns_error_on_image_acquire_fail);
	add_test(swc, render_returns_error_on_submit_fail);
	add_test(swc, render_returns_error_on_present_fail);
	add_test(swc, terminate_destroys_all_resources);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(swc, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(swc);
	return exit_code;
}
