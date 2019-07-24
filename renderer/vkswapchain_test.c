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

#include "vkswapchain.h"
#include "vkrenderer.h"

Ensure(init_returns_zero_on_success)
{
	struct vkrenderer vkr = { 0 };
	struct vkswapchain swc = { 0 };
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	uint32_t nimgs = 1;
	expect(vkGetSwapchainImagesKHR,
	       will_set_contents_of_parameter(pSwapchainImageCount, &nimgs,
					      sizeof(nimgs)),
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
					      sizeof(nimgs)),
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
					      sizeof(image_index)),
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
					      sizeof(image_index)),
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
