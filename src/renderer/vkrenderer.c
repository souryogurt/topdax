/**
 * @file
 * Vulkan renderer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include <application/utils.h>
#include <renderer/vkrenderer.h>
#include "vkrenderer.h"

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
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkswapchain_create(VkSwapchainKHR * swapchain,
				   const struct vkrenderer *rdr)
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
	swc->nframes = ARRAY_SIZE(images);
	VkResult result = vkGetSwapchainImagesKHR(rdr->device, swc->swapchain,
						  &swc->nframes, images);
	if (result != VK_SUCCESS)
		return -1;
	for (size_t i = 0; i < swc->nframes; ++i) {
		if (vkframe_init(&swc->frames[i], rdr, images[i]) != VK_SUCCESS) {
			return -1;
		}
	}
	return 0;
}

/**
 * Initializes renderpass for renderer
 * @param swc Specifies swapchain to create renderpass for
 * @param rdr Specifies renderer to get params from
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkswapchain_init_render_pass(struct vkswapchain *swc,
					     const struct vkrenderer *rdr)
{
	VkAttachmentDescription attachments[] = {
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
	VkAttachmentReference attachment_refs[] = {
		{
		 .attachment = 0,
		 .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		 },
	};
	VkSubpassDescription subpasses[] = {
		{
		 .flags = 0,
		 .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		 .inputAttachmentCount = 0,
		 .pInputAttachments = NULL,
		 .colorAttachmentCount = ARRAY_SIZE(attachment_refs),
		 .pColorAttachments = attachment_refs,
		 .pResolveAttachments = NULL,
		 .pDepthStencilAttachment = NULL,
		 .preserveAttachmentCount = 0,
		 .pPreserveAttachments = NULL,
		 },
	};
	VkSubpassDependency dependencies[] = {
		{
		 .srcSubpass = VK_SUBPASS_EXTERNAL,
		 .dstSubpass = 0,
		 .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		 .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		 .srcAccessMask = 0,
		 .dstAccessMask =
		 VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
		 VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		 .dependencyFlags = 0,
		 },
	};
	VkRenderPassCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.attachmentCount = ARRAY_SIZE(attachments),
		.pAttachments = attachments,
		.subpassCount = ARRAY_SIZE(subpasses),
		.pSubpasses = subpasses,
		.dependencyCount = ARRAY_SIZE(dependencies),
		.pDependencies = dependencies,
	};
	return vkCreateRenderPass(rdr->device, &info, NULL, &swc->renderpass);
}

/**
 * Initialize swapchain struct
 * @param swc Specifies pointer to vkswapchain struct to initialize
 * @param rdr Specifies renderer to get parameters from
 * @returns zero on sucess, or non-zero otherwise
 */
static int vkswapchain_init(struct vkswapchain *swc,
			    const struct vkrenderer *rdr)
{
	if (vkswapchain_create(&swc->swapchain, rdr) != VK_SUCCESS) {
		return -1;
	}
	if (vkswapchain_init_render_pass(swc, rdr) != VK_SUCCESS) {
		return -1;
	}
	if (vkswapchain_create_sync_objects(swc, rdr->device) != VK_SUCCESS) {
		return -1;
	}
	return vkswapchain_init_frames(swc, rdr) != VK_SUCCESS;
}

/**
 * Terminate swapchain
 * @param swc Specifies pointer to vkswapchain to terminate
 * @param dev Spwcifies Vulkan device to remove swapchain from
 */
static void vkswapchain_terminate(const struct vkswapchain *swc, VkDevice dev)
{
	for (size_t i = 0; i < swc->nframes; ++i) {
		vkframe_destroy(&swc->frames[i], dev);
	}
	vkDestroySemaphore(dev, swc->render_sem, NULL);
	vkDestroySemaphore(dev, swc->acquire_sem, NULL);
	vkDestroyRenderPass(dev, swc->renderpass, NULL);
	vkDestroySwapchainKHR(dev, swc->swapchain, NULL);
}

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
