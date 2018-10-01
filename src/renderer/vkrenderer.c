/**
 * @file
 * Vulkan renderer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <application/utils.h>
#include <renderer/vkrenderer.h>
#include <string.h>
#include "vkrenderer.h"

/**
 * Create Vulkan device for renderer
 * @param rdr Specifies renderer to create device for
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkrenderer_create_device(struct vkrenderer *rdr)
{
	float queue_priorities = 1.0f;
	VkDeviceQueueCreateInfo qinfos[2] = {
		{
		 .queueFamilyIndex = rdr->graphic,
		 },
		{
		 .queueFamilyIndex = rdr->present,
		 },
	};
	uint32_t nqinfos = (rdr->graphic == rdr->present) ? 1 : 2;
	for (size_t i = 0; i < nqinfos; i++) {
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
 * Create Vulkan swapchain for renderer
 * @param rdr Specifies renderer to create swapchain for
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkrenderer_create_swapchain(struct vkrenderer *rdr)
{
	uint32_t indeces[] = { rdr->graphic, rdr->present };
	uint32_t nindeces = ARRAY_SIZE(indeces);
	VkSharingMode sharing_mode = (rdr->graphic == rdr->present) ?
	    VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
	uint32_t image_count = rdr->srf_caps.minImageCount + 1;
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
		.oldSwapchain = VK_NULL_HANDLE,
	};
	return vkCreateSwapchainKHR(rdr->device, &info, NULL, &rdr->swapchain);
}

static int vkrenderer_init_frame_view(struct vkrenderer *rdr, size_t index)
{
	VkImageViewCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.image = rdr->frames[index],
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = rdr->srf_format.format,
		.components = {
			       .r = VK_COMPONENT_SWIZZLE_IDENTITY,
			       .g = VK_COMPONENT_SWIZZLE_IDENTITY,
			       .b = VK_COMPONENT_SWIZZLE_IDENTITY,
			       .a = VK_COMPONENT_SWIZZLE_IDENTITY,
			       },
		.subresourceRange = {
				     .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				     .baseMipLevel = 0,
				     .levelCount = 1,
				     .baseArrayLayer = 0,
				     .layerCount = 1,
				     },
	};
	return vkCreateImageView(rdr->device, &info, NULL,
				 &rdr->frame_views[index]);
}

static int vkrenderer_init_frames(struct vkrenderer *rdr)
{
	rdr->nframes = ARRAY_SIZE(rdr->frames);
	VkResult result = vkGetSwapchainImagesKHR(rdr->device, rdr->swapchain,
						  &rdr->nframes, rdr->frames);
	if (result != VK_SUCCESS)
		return -1;
	for (size_t i = 0; i < rdr->nframes; i++) {
		if (vkrenderer_init_frame_view(rdr, i) != VK_SUCCESS) {
			return -1;
		}
	}
	return 0;
}

int vkrenderer_init(struct vkrenderer *rdr, VkInstance instance,
		    VkSurfaceKHR surface)
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
	if (vkrenderer_create_swapchain(rdr) != VK_SUCCESS) {
		return -1;
	}
	return vkrenderer_init_frames(rdr);
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	for (size_t i = 0; i < rdr->nframes; i++) {
		vkDestroyImageView(rdr->device, rdr->frame_views[i], NULL);
	}
	vkDestroySwapchainKHR(rdr->device, rdr->swapchain, NULL);
	vkDestroyDevice(rdr->device, NULL);
}
