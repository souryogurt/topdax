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

/**
 * Initializes swapchain images
 * @param rdr Specifies renderer to initialize images for
 * @returns zero on success, or non-zero otherwise
 */
static int vkrenderer_init_frames(struct vkrenderer *rdr)
{
	VkImage images[ARRAY_SIZE(rdr->frames)];
	rdr->nframes = ARRAY_SIZE(images);
	VkResult result = vkGetSwapchainImagesKHR(rdr->device, rdr->swapchain,
						  &rdr->nframes, images);
	if (result != VK_SUCCESS)
		return -1;
	for (size_t i = 0; i < rdr->nframes; i++) {
		if (vkframe_init(&rdr->frames[i], rdr, images[i]) != VK_SUCCESS) {
			return -1;
		}
	}
	return 0;
}

/**
 * Initializes renderpass for renderer
 * @param rdr Specifies renderer to create renderpass for
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkrenderer_init_render_pass(struct vkrenderer *rdr)
{
	VkAttachmentDescription color_attachment[] = {
		{
		 .flags = 0,
		 .format = rdr->srf_format.format,
		 .samples = VK_SAMPLE_COUNT_1_BIT,
		 .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		 .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		 .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		 .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		 .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		 .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		 },
	};
	VkAttachmentReference color_attachment_ref[] = {
		{
		 .attachment = 0,
		 .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		 },
	};
	VkSubpassDescription subpass[] = {
		{
		 .flags = 0,
		 .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		 .inputAttachmentCount = 0,
		 .pInputAttachments = NULL,
		 .colorAttachmentCount = ARRAY_SIZE(color_attachment_ref),
		 .pColorAttachments = color_attachment_ref,
		 .pResolveAttachments = NULL,
		 .pDepthStencilAttachment = NULL,
		 .preserveAttachmentCount = 0,
		 .pPreserveAttachments = NULL,
		 },
	};
	VkRenderPassCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.attachmentCount = ARRAY_SIZE(color_attachment),
		.pAttachments = color_attachment,
		.subpassCount = ARRAY_SIZE(subpass),
		.pSubpasses = subpass,
		.dependencyCount = 0,
		.pDependencies = NULL,
	};
	return vkCreateRenderPass(rdr->device, &info, NULL, &rdr->renderpass);
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
	if (vkrenderer_init_render_pass(rdr) != VK_SUCCESS) {
		return -1;
	}
	if (vkrenderer_init_command_pool(rdr) != VK_SUCCESS) {
		return -1;
	}

	return vkrenderer_init_frames(rdr) != VK_SUCCESS;
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	for (size_t i = 0; i < rdr->nframes; i++) {
		vkframe_destroy(&rdr->frames[i], rdr->device);
	}
	vkDestroyCommandPool(rdr->device, rdr->cmd_pool, NULL);
	vkDestroyRenderPass(rdr->device, rdr->renderpass, NULL);
	vkDestroySwapchainKHR(rdr->device, rdr->swapchain, NULL);
	vkDestroyDevice(rdr->device, NULL);
}
