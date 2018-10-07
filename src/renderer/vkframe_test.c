/**
 * @file
 * Test suite for vkframe
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <renderer/vkrenderer.h>

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateImageView(VkDevice device, const VkImageViewCreateInfo * pCreateInfo,
		  const VkAllocationCallbacks * pAllocator, VkImageView * pView)
{
	return (VkResult) mock(device, pCreateInfo, pAllocator, pView);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyImageView(VkDevice device, VkImageView imageView,
		   const VkAllocationCallbacks * pAllocator)
{
	mock(device, imageView, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateFramebuffer(VkDevice device,
		    const VkFramebufferCreateInfo * pCreateInfo,
		    const VkAllocationCallbacks * pAllocator,
		    VkFramebuffer * pFramebuffer)
{
	return (VkResult) mock(device, pCreateInfo, pAllocator, pFramebuffer);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer,
		     const VkAllocationCallbacks * pAllocator)
{
	mock(device, framebuffer, pAllocator);
}

Ensure(vkframe_init_returns_error_on_framebuffer_fail)
{
	struct vkframe frame;
	struct vkrenderer rdr;
	VkImage image = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_SUCCESS));
	expect(vkCreateFramebuffer, will_return(VK_NOT_READY));
	int error = vkframe_init(&frame, &rdr, image);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkframe_init_returns_error_on_getting_view_fail)
{
	struct vkframe frame;
	struct vkrenderer rdr;
	VkImage image = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_NOT_READY));
	int error = vkframe_init(&frame, &rdr, image);
	assert_that(error, is_equal_to(VK_NOT_READY));
}

Ensure(vkframe_init_returns_vk_success_on_success)
{
	struct vkframe frame;
	struct vkrenderer rdr;
	VkImage image = VK_NULL_HANDLE;
	expect(vkCreateImageView, will_return(VK_SUCCESS));
	expect(vkCreateFramebuffer, will_return(VK_SUCCESS));
	int error = vkframe_init(&frame, &rdr, image);
	assert_that(error, is_equal_to(VK_SUCCESS));
}

Ensure(vkframe_destroy_destroys_all_resources)
{
	struct vkframe frame;
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
	add_test(vkf, vkframe_destroy_destroys_all_resources);
	return run_test_suite(vkf, create_text_reporter());
}
