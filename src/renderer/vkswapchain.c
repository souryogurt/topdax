/**
 * @file
 * Vulkan swapchain implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stddef.h>
#include <stdint.h>

#include <application/utils.h>
#include "renderer/vkframe.h"
#include <renderer/vkrenderer.h>
#include <renderer/vkswapchain.h>
#include <vulkan/vulkan_core.h>

/**
 * Create synchronization objects used during rendering
 * @param swc Specifies swapchain to create sync objects for
 * @param dev Specifies device to create sync objects on
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkswapchain_create_sync_objects(struct vkswapchain *swc,
						VkDevice dev)
{
	VkResult result;
	VkSemaphoreCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
	};
	result = vkCreateSemaphore(dev, &info, NULL, &swc->acquire_sem);
	if (result != VK_SUCCESS)
		return result;
	return vkCreateSemaphore(dev, &info, NULL, &swc->render_sem);
}

/**
 * Create Vulkan swapchain for renderer
 * @param swapchain Specifies pointer to destination variable of swapchain
 * @param rdr Specifies renderer to use for creation
 * @param old_swc Specifies handle of old swapchain, or VK_NULL_HANDLE
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkswapchain_create(VkSwapchainKHR * swapchain,
				   const struct vkrenderer *rdr,
				   const VkSwapchainKHR old_swc)
{
	uint32_t indeces[] = { rdr->graphic, rdr->present };
	uint32_t nindeces = ARRAY_SIZE(indeces);
	VkSharingMode sharing_mode = (rdr->graphic == rdr->present) ?
	    VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
	uint32_t image_count = rdr->srf_caps.minImageCount;
	VkSwapchainCreateInfoKHR info = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.pNext = NULL,
		.flags = 0,
		.surface = rdr->srf,
		.minImageCount = image_count,
		.imageFormat = rdr->srf_format.format,
		.imageColorSpace = rdr->srf_format.colorSpace,
		.imageExtent = rdr->srf_caps.currentExtent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = sharing_mode,
		.queueFamilyIndexCount = nindeces,
		.pQueueFamilyIndices = indeces,
		.preTransform = rdr->srf_caps.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = rdr->srf_mode,
		.clipped = VK_TRUE,
		.oldSwapchain = old_swc,
	};
	return vkCreateSwapchainKHR(rdr->device, &info, NULL, swapchain);
}

/**
 * Initializes swapchain images
 * @param swc Specifies swapchain to initialize images for
 * @param rdr Specifies renderer to to get images from
 * @returns zero on success, or non-zero otherwise
 */
static int vkswapchain_init_frames(struct vkswapchain *swc,
				   const struct vkrenderer *rdr)
{
	VkImage images[ARRAY_SIZE(swc->frames)];
	uint32_t nimages = ARRAY_SIZE(images);
	VkResult err = vkGetSwapchainImagesKHR(rdr->device, swc->swapchain,
					       &nimages, images);
	if (err != VK_SUCCESS)
		return -1;

	for (swc->nframes = 0; swc->nframes < nimages; ++swc->nframes) {
		struct vkframe *frame = &swc->frames[swc->nframes];
		VkImage image = images[swc->nframes];
		err = vkframe_init(frame, rdr->rpass, rdr, image);
		if (err != VK_SUCCESS) {
			return -1;
		}
	}
	return 0;
}

int vkswapchain_init(struct vkswapchain *swc, const struct vkrenderer *rdr,
		     const VkSwapchainKHR old_swc)
{
	if (vkswapchain_create(&swc->swapchain, rdr, old_swc) != VK_SUCCESS) {
		return -1;
	}
	const VkDevice dev = rdr->device;
	if (vkswapchain_create_sync_objects(swc, dev) != VK_SUCCESS) {
		return -1;
	}
	return vkswapchain_init_frames(swc, rdr) != VK_SUCCESS;
}

VkResult vkswapchain_render(const struct vkswapchain *swc,
			    const struct vkrenderer *rdr)
{
	uint32_t image_index;
	VkResult result = vkAcquireNextImageKHR(rdr->device, swc->swapchain,
						UINT64_MAX, swc->acquire_sem,
						VK_NULL_HANDLE, &image_index);
	if (result != VK_SUCCESS)
		return result;
	const VkPipelineStageFlags wait_stages[] = {
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	};
	VkSubmitInfo submit_info = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = NULL,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &swc->acquire_sem,
		.pWaitDstStageMask = wait_stages,
		.commandBufferCount = 1,
		.pCommandBuffers = &swc->frames[image_index].cmds,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &swc->render_sem,
	};
	result = vkQueueSubmit(rdr->graphics_queue, 1, &submit_info,
			       VK_NULL_HANDLE);
	if (result != VK_SUCCESS)
		return result;
	VkPresentInfoKHR present_info = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.pNext = NULL,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &swc->render_sem,
		.swapchainCount = 1,
		.pSwapchains = &swc->swapchain,
		.pImageIndices = &image_index,
		.pResults = NULL,
	};
	return vkQueuePresentKHR(rdr->present_queue, &present_info);
}

void vkswapchain_terminate(const struct vkswapchain *swc, VkDevice dev)
{
	for (size_t i = 0; i < swc->nframes; ++i) {
		vkframe_destroy(&swc->frames[i], dev);
	}
	vkDestroySemaphore(dev, swc->render_sem, NULL);
	vkDestroySemaphore(dev, swc->acquire_sem, NULL);
	vkDestroySwapchainKHR(dev, swc->swapchain, NULL);
}
