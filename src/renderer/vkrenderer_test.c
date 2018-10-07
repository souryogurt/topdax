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

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo * pCreateInfo,
		   const VkAllocationCallbacks * pAllocator,
		   VkRenderPass * pRenderPass)
{
	return (VkResult) mock(device, pCreateInfo, pAllocator, pRenderPass);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass,
		    const VkAllocationCallbacks * pAllocator)
{
	mock(device, renderPass, pAllocator);
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

VkResult
vkframe_init(struct vkframe *frame, struct vkrenderer *rdr, VkImage image)
{
	return (VkResult) mock(frame, rdr, image);
}

void vkframe_destroy(struct vkframe *frame, VkDevice device)
{
	mock(frame, device);
}

Ensure(vkrenderer_init_returns_zero_on_success)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	expect(vkCreateRenderPass, will_return(VK_SUCCESS));
	uint32_t nimgs = 1;
	expect(vkGetSwapchainImagesKHR,
	       will_set_contents_of_parameter(pSwapchainImageCount, &nimgs,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	expect(vkframe_init, will_return(VK_SUCCESS));
	expect(vkCreateCommandPool, will_return(VK_SUCCESS));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_when_no_configs)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(vkrenderer_configure, will_return(1));
	never_expect(vkCreateDevice, will_return(VK_SUCCESS));
	never_expect(vkGetDeviceQueue);
	never_expect(vkCreateSwapchainKHR);
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_on_device_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_ERROR_INITIALIZATION_FAILED));
	never_expect(vkGetDeviceQueue);
	never_expect(vkCreateSwapchainKHR);
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_on_swapchain_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateSwapchainKHR,
	       will_return(VK_ERROR_INITIALIZATION_FAILED));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_on_getting_images_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	expect(vkCreateRenderPass, will_return(VK_SUCCESS));
	expect(vkGetSwapchainImagesKHR, will_return(VK_INCOMPLETE));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_on_frame_init_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	expect(vkCreateRenderPass, will_return(VK_SUCCESS));
	uint32_t nimgs = 1;
	expect(vkGetSwapchainImagesKHR,
	       will_set_contents_of_parameter(pSwapchainImageCount, &nimgs,
					      sizeof(uint32_t)),
	       will_return(VK_SUCCESS));
	expect(vkframe_init, will_return(VK_ERROR_INITIALIZATION_FAILED));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_init_returns_non_zero_on_renderpass_fail)
{
	VkInstance instance = (VkInstance) 1;
	VkSurfaceKHR surface = (VkSurfaceKHR) 2;
	struct vkrenderer vkr;
	expect(vkrenderer_configure, will_return(0));
	expect(vkCreateDevice, will_return(VK_SUCCESS));
	expect(vkGetDeviceQueue);
	expect(vkGetDeviceQueue);
	expect(vkCreateSwapchainKHR, will_return(VK_SUCCESS));
	expect(vkCreateRenderPass, will_return(VK_NOT_READY));
	int error = vkrenderer_init(&vkr, instance, surface);
	assert_that(error, is_not_equal_to(0));
}

Ensure(vkrenderer_terminate_destroys_all_resources)
{
	struct vkrenderer vkr = {
		.nframes = 1,
	};
	expect(vkDestroyCommandPool);
	expect(vkframe_destroy);
	expect(vkDestroyRenderPass);
	expect(vkDestroySwapchainKHR);
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
	add_test(vkr, vkrenderer_init_returns_non_zero_on_swapchain_fail);
	add_test(vkr, vkrenderer_init_returns_non_zero_on_getting_images_fail);
	add_test(vkr, vkrenderer_init_returns_non_zero_on_frame_init_fail);
	add_test(vkr, vkrenderer_init_returns_non_zero_on_renderpass_fail);
	add_test(vkr, vkrenderer_terminate_destroys_all_resources);
	return run_test_suite(vkr, create_text_reporter());
}
