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

#include "vkrenderer.h"
#include "config.h"

Ensure(init_returns_zero_on_success)
{
	VkInstance instance = (VkInstance)1;
	VkSurfaceKHR surface = (VkSurfaceKHR)2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateCommandPool, will_return(VK_SUCCESS));
	expect(vkCreateRenderPass, will_return(VK_SUCCESS));
	expect(vkswapchain_init, will_return(0));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_equal_to(0));
}

Ensure(init_returns_non_zero_when_no_configs)
{
	VkInstance instance = (VkInstance)1;
	VkSurfaceKHR surface = (VkSurfaceKHR)2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(1));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_device_fail)
{
	VkInstance instance = (VkInstance)1;
	VkSurfaceKHR surface = (VkSurfaceKHR)2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_ERROR_INITIALIZATION_FAILED));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_command_pool_fail)
{
	VkInstance instance = (VkInstance)1;
	VkSurfaceKHR surface = (VkSurfaceKHR)2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateCommandPool, will_return(VK_NOT_READY));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_renderpass_fail)
{
	VkInstance instance = (VkInstance)1;
	VkSurfaceKHR surface = (VkSurfaceKHR)2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateCommandPool, will_return(VK_SUCCESS));
	expect(vkCreateRenderPass, will_return(VK_NOT_READY));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(init_returns_non_zero_on_swapchain_fail)
{
	VkInstance instance = (VkInstance)1;
	VkSurfaceKHR surface = (VkSurfaceKHR)2;
	struct vkrenderer vkr = { 0 };
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateCommandPool, will_return(VK_SUCCESS));
	expect(vkCreateRenderPass, will_return(VK_SUCCESS));
	expect(vkswapchain_init, will_return(-1));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(render_returns_zero_on_success)
{
	struct vkrenderer vkr = { 0 };
	expect(vkswapchain_render, when(swc, is_equal_to(&vkr.swcs[0])),
	       when(rdr, is_equal_to(&vkr)), will_return(VK_SUCCESS));
	int error = vkrenderer_render(&vkr);
	assert_that(error, is_equal_to(0));
}

Ensure(render_recreates_swapchain)
{
	struct vkrenderer vkr = { 0 };
	expect(vkswapchain_render, when(swc, is_equal_to(&vkr.swcs[0])),
	       when(rdr, is_equal_to(&vkr)),
	       will_return(VK_ERROR_OUT_OF_DATE_KHR));
	expect(vkDeviceWaitIdle, when(device, is_equal_to(vkr.device)));
	expect(vkrenderer_configure_swapchain, will_return(0),
	       when(rdr, is_equal_to(&vkr)));
	expect(vkswapchain_init, will_return(0),
	       when(swc, is_equal_to(&vkr.swcs[1])));
	expect(vkswapchain_terminate, when(swc, is_equal_to(&vkr.swcs[0])));
	expect(vkswapchain_render, when(swc, is_equal_to(&vkr.swcs[1])),
	       when(rdr, is_equal_to(&vkr)), will_return(VK_SUCCESS));
	int error = vkrenderer_render(&vkr);
	assert_that(error, is_equal_to(0));
	assert_that(vkr.swc_index, is_equal_to(1));
}

Ensure(render_returns_non_zero_on_swapchain_config_fail)
{
	struct vkrenderer vkr = { 0 };
	expect(vkswapchain_render, when(swc, is_equal_to(&vkr.swcs[0])),
	       when(rdr, is_equal_to(&vkr)),
	       will_return(VK_ERROR_OUT_OF_DATE_KHR));
	expect(vkDeviceWaitIdle, when(device, is_equal_to(vkr.device)));
	expect(vkrenderer_configure_swapchain, will_return(1),
	       when(rdr, is_equal_to(&vkr)));
	int error = vkrenderer_render(&vkr);
	assert_that(error, is_not_equal_to(0));
	assert_that(vkr.swc_index, is_equal_to(0));
}

Ensure(render_returns_non_zero_on_swapchain_init_fail)
{
	struct vkrenderer vkr = { 0 };
	expect(vkswapchain_render, when(swc, is_equal_to(&vkr.swcs[0])),
	       when(rdr, is_equal_to(&vkr)),
	       will_return(VK_ERROR_OUT_OF_DATE_KHR));
	expect(vkDeviceWaitIdle, when(device, is_equal_to(vkr.device)));
	expect(vkrenderer_configure_swapchain, will_return(0),
	       when(rdr, is_equal_to(&vkr)));
	expect(vkswapchain_init, will_return(1),
	       when(swc, is_equal_to(&vkr.swcs[1])));
	int error = vkrenderer_render(&vkr);
	assert_that(error, is_not_equal_to(0));
	assert_that(vkr.swc_index, is_equal_to(0));
}

Ensure(terminate_destroys_all_resources)
{
	struct vkrenderer vkr = { 0 };
	expect(vkDeviceWaitIdle);
	expect(vkDestroyRenderPass);
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
	add_test(vkr, init_returns_non_zero_on_renderpass_fail);
	add_test(vkr, init_returns_non_zero_on_swapchain_fail);
	add_test(vkr, render_returns_zero_on_success);
	add_test(vkr, render_recreates_swapchain);
	add_test(vkr, render_returns_non_zero_on_swapchain_config_fail);
	add_test(vkr, render_returns_non_zero_on_swapchain_init_fail);
	add_test(vkr, terminate_destroys_all_resources);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(vkr, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(vkr);
	return exit_code;
}
