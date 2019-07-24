/**
 * @file
 * Test suite for vkframe
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <vulkan/vulkan_core.h>
#include "vkframe.h"
#include "vkrenderer.h"

Ensure(vkframe_init_returns_error_on_framebuffer_fail)
{
	struct vkframe frame;
	struct vkrenderer rdr = { 0 };
	VkImage image = VK_NULL_HANDLE;
	VkRenderPass rpass = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_SUCCESS));
	expect(vkCreateFramebuffer, will_return(VK_NOT_READY));
	int error = vkframe_init(&frame, rpass, &rdr, image);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkframe_init_returns_error_on_getting_view_fail)
{
	struct vkframe frame;
	struct vkrenderer rdr = { 0 };
	VkImage image = VK_NULL_HANDLE;
	VkRenderPass rpass = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_NOT_READY));
	int error = vkframe_init(&frame, rpass, &rdr, image);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkframe_init_returns_error_on_command_buffer_fail)
{
	struct vkframe frame;
	struct vkrenderer rdr = { 0 };
	VkImage image = VK_NULL_HANDLE;
	VkRenderPass rpass = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_SUCCESS));
	expect(vkCreateFramebuffer, will_return(VK_SUCCESS));
	expect(vkAllocateCommandBuffers, will_return(VK_NOT_READY));
	int error = vkframe_init(&frame, rpass, &rdr, image);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkframe_init_returns_vk_success_on_success)
{
	struct vkframe frame;
	struct vkrenderer rdr = { 0 };
	VkImage image = VK_NULL_HANDLE;
	VkRenderPass rpass = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_SUCCESS));
	expect(vkCreateFramebuffer, will_return(VK_SUCCESS));
	expect(vkAllocateCommandBuffers, will_return(VK_SUCCESS));
	expect(vkBeginCommandBuffer, will_return(VK_SUCCESS));
	expect(vkCmdBeginRenderPass, will_return(VK_SUCCESS));
	expect(vkCmdEndRenderPass, will_return(VK_SUCCESS));
	expect(vkEndCommandBuffer, will_return(VK_SUCCESS));
	int error = vkframe_init(&frame, rpass, &rdr, image);
	assert_that(error, is_equal_to(VK_SUCCESS));
}

Ensure(vkframe_init_returns_error_on_end_cmd_buffer)
{
	struct vkframe frame;
	struct vkrenderer rdr = { 0 };
	VkImage image = VK_NULL_HANDLE;
	VkRenderPass rpass = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_SUCCESS));
	expect(vkCreateFramebuffer, will_return(VK_SUCCESS));
	expect(vkAllocateCommandBuffers, will_return(VK_SUCCESS));
	expect(vkBeginCommandBuffer, will_return(VK_SUCCESS));
	expect(vkCmdBeginRenderPass, will_return(VK_SUCCESS));
	expect(vkCmdEndRenderPass, will_return(VK_SUCCESS));
	expect(vkEndCommandBuffer, will_return(VK_NOT_READY));
	int error = vkframe_init(&frame, rpass, &rdr, image);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkframe_init_returns_error_on_begin_cmd_buffer)
{
	struct vkframe frame;
	struct vkrenderer rdr = { 0 };
	VkImage image = VK_NULL_HANDLE;
	VkRenderPass rpass = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_SUCCESS));
	expect(vkCreateFramebuffer, will_return(VK_SUCCESS));
	expect(vkAllocateCommandBuffers, will_return(VK_SUCCESS));
	expect(vkBeginCommandBuffer, will_return(VK_NOT_READY));
	int error = vkframe_init(&frame, rpass, &rdr, image);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkframe_destroy_destroys_all_resources)
{
	struct vkframe frame = { 0 };
	VkDevice device = VK_NULL_HANDLE;
	expect(vkDestroyFramebuffer);
	expect(vkDestroyImageView);
	vkframe_destroy(&frame, device);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkf = create_named_test_suite("VKFrame");
	add_test(vkf, vkframe_init_returns_vk_success_on_success);
	add_test(vkf, vkframe_init_returns_error_on_getting_view_fail);
	add_test(vkf, vkframe_init_returns_error_on_framebuffer_fail);
	add_test(vkf, vkframe_init_returns_error_on_command_buffer_fail);
	add_test(vkf, vkframe_init_returns_error_on_begin_cmd_buffer);
	add_test(vkf, vkframe_init_returns_error_on_end_cmd_buffer);
	add_test(vkf, vkframe_destroy_destroys_all_resources);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(vkf, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(vkf);
	return exit_code;
}
