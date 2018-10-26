/**
 * @file
 * Vulkan renderer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stddef.h>
#include <stdint.h>

#include <renderer/vkrenderer.h>
#include "renderer/vkframe.h"
#include "renderer/vkswapchain.h"
#include "vkrenderer.h"

/**
 * Create Vulkan device for renderer
 * @param rdr Specifies renderer to create device for
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkrenderer_create_device(struct vkrenderer *rdr)
{
	float queue_priorities = 1.0F;
	VkDeviceQueueCreateInfo qinfos[2] = {
		{
		 .queueFamilyIndex = rdr->graphic,
		 },
		{
		 .queueFamilyIndex = rdr->present,
		 },
	};
	uint32_t nqinfos = (rdr->graphic == rdr->present) ? 1 : 2;
	for (size_t i = 0; i < nqinfos; ++i) {
		VkDeviceQueueCreateInfo *info = &qinfos[i];
		info->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info->pNext = NULL;
		info->flags = 0;
		info->queueCount = 1;
		info->pQueuePriorities = &queue_priorities;
	}
	VkDeviceCreateInfo dev_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.queueCreateInfoCount = nqinfos,
		.pQueueCreateInfos = qinfos,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = rdr->nextensions,
		.ppEnabledExtensionNames = rdr->extensions,
		.pEnabledFeatures = &rdr->features,
	};
	return vkCreateDevice(rdr->phy, &dev_info, NULL, &rdr->device);
}

/**
 * Initialize command pool for renderer
 * @param rdr Specifies renderer to initialize command pool for
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkrenderer_init_command_pool(struct vkrenderer *rdr)
{
	VkCommandPoolCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.queueFamilyIndex = rdr->graphic,
	};
	return vkCreateCommandPool(rdr->device, &info, NULL, &rdr->cmd_pool);
}

int vkrenderer_init(struct vkrenderer *rdr, VkInstance instance,
		    const VkSurfaceKHR surface)
{
	rdr->srf = surface;
	if (vkrenderer_configure(rdr, instance)) {
		return -1;
	}
	if (vkrenderer_create_device(rdr) != VK_SUCCESS) {
		return -1;
	}
	vkGetDeviceQueue(rdr->device, rdr->graphic, 0, &rdr->graphics_queue);
	vkGetDeviceQueue(rdr->device, rdr->present, 0, &rdr->present_queue);
	if (vkrenderer_init_command_pool(rdr) != VK_SUCCESS) {
		return -1;
	}
	return vkswapchain_init(&rdr->swapchain, rdr);
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

void vkrenderer_terminate(const struct vkrenderer *rdr)
{
	vkDeviceWaitIdle(rdr->device);
	vkswapchain_terminate(&rdr->swapchain, rdr->device);
	vkDestroyCommandPool(rdr->device, rdr->cmd_pool, NULL);
	vkDestroyDevice(rdr->device, NULL);
}
