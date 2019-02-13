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

/**
 * Initializes renderpass for renderer
 * @param rpass Specifies renderpass to initialize
 * @param format Specifies format of render targets
 * @param dev Specifies device to use
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkrenderer_init_render_pass(VkRenderPass * rpass,
					    const VkFormat format,
					    const VkDevice dev)
{
	VkAttachmentDescription attachments[] = {
		{
		 .flags = 0,
		 .format = format,
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
	return vkCreateRenderPass(dev, &info, NULL, rpass);
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
	const VkFormat fmt = rdr->srf_format.format;
	const VkDevice dev = rdr->device;
	if (vkrenderer_init_render_pass(&rdr->rpass, fmt, dev) != VK_SUCCESS) {
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
	vkDestroyRenderPass(rdr->device, rdr->rpass, NULL);
	vkDestroyCommandPool(rdr->device, rdr->cmd_pool, NULL);
	vkDestroyDevice(rdr->device, NULL);
}
