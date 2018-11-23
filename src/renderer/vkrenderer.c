/**
 * @file
 * Vulkan renderer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stddef.h>
#include <stdint.h>

#include <application/utils.h>
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

	rdr->swc_index = 0;
	return vkswapchain_init(&rdr->swcs[rdr->swc_index], rdr,
				VK_NULL_HANDLE);
}

int vkrenderer_render(struct vkrenderer *rdr)
{
	struct vkswapchain *swc = &rdr->swcs[rdr->swc_index];
	VkResult result = vkswapchain_render(swc, rdr);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		vkDeviceWaitIdle(rdr->device);
		size_t swc_index = (rdr->swc_index + 1) % ARRAY_SIZE(rdr->swcs);
		struct vkswapchain *new_swc = &rdr->swcs[swc_index];
		if (vkrenderer_configure_swapchain(rdr)) {
			return -1;
		}
		if (vkswapchain_init(new_swc, rdr, swc->swapchain)) {
			return -1;
		}
		vkswapchain_terminate(swc, rdr->device);
		rdr->swc_index = swc_index;
		swc = new_swc;
		result = vkswapchain_render(swc, rdr);
	}
	return result != VK_SUCCESS;
}

void vkrenderer_terminate(const struct vkrenderer *rdr)
{
	vkDeviceWaitIdle(rdr->device);
	vkswapchain_terminate(&rdr->swcs[rdr->swc_index], rdr->device);
	vkDestroyCommandPool(rdr->device, rdr->cmd_pool, NULL);
	vkDestroyDevice(rdr->device, NULL);
}
