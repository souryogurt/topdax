/**
 * @file
 * Vulkan frame implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stddef.h>

#include <application/utils.h>
#include "renderer/vkframe.h"
#include <renderer/vkrenderer.h>
#include "renderer/vkswapchain.h"
#include <vulkan/vulkan_core.h>

/**
 * Initializes framebuffer on frame's image
 * @param frame Specifies frame to initialize framebuffer for
 * @param rpass Specifies render pass the framebuffer will be compatible with
 * @param rdr Specifies renderer of this frame
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkframe_init_framebuffer(struct vkframe *frame,
					 const VkRenderPass rpass,
					 const VkDevice device)
{

	const VkImageView attachments[] = { frame->view };
	const VkFramebufferCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.renderPass = rpass,
		.attachmentCount = ARRAY_SIZE(attachments),
		.pAttachments = attachments,
		.width = frame->size.width,
		.height = frame->size.height,
		.layers = 1,
	};
	return vkCreateFramebuffer(device, &info, NULL, &frame->buffer);
}

/**
 * Initializes image view on frame's image
 * @param frame Specifies frame to initialize view for
 * @param format Specifies format of underlying image
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkframe_init_view(struct vkframe *frame, const VkFormat format,
				  const VkDevice device)
{
	const VkImageSubresourceRange base_range = {
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};

	const VkComponentMapping identity_mapping = {
		.r = VK_COMPONENT_SWIZZLE_IDENTITY,
		.g = VK_COMPONENT_SWIZZLE_IDENTITY,
		.b = VK_COMPONENT_SWIZZLE_IDENTITY,
		.a = VK_COMPONENT_SWIZZLE_IDENTITY,
	};

	const VkImageViewCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.image = frame->image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
		.components = identity_mapping,
		.subresourceRange = base_range,
	};
	return vkCreateImageView(device, &info, NULL, &frame->view);
}

/**
 * Allocates primary command bufffer for frame
 * @param frame Specifies frame to allocate primary command buffer for
 * @param cmd_pool Specifies commandbuffer pool to allocate buffer from
 * @param device Specifies device to use
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkframe_alloc_cmds(struct vkframe *frame,
				   const VkCommandPool cmd_pool,
				   const VkDevice device)
{
	const VkCommandBufferAllocateInfo info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext = NULL,
		.commandPool = cmd_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	return vkAllocateCommandBuffers(device, &info, &frame->cmds);
}

/**
 * Records commands to frame
 * @param frame Specifies the frame to record commands for
 * @param rpass Specifies the renderpass to record
 * @returns VK_SUCCESS on success, or error otherwise
 */
static VkResult vkframe_record(const struct vkframe *frame,
			       const VkRenderPass rpass)
{
	const VkCommandBufferBeginInfo begin_info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = NULL,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = NULL,
	};
	VkResult result = vkBeginCommandBuffer(frame->cmds, &begin_info);
	if (result != VK_SUCCESS)
		return result;
	const VkClearValue clear_colors[] = {
		{{{1.0F, 1.0F, 1.0F, 1.0F}}}
	};
	const VkRect2D render_rect = {
		.offset = {0, 0},
		.extent = frame->size,
	};

	const VkRenderPassBeginInfo rbf = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.pNext = NULL,
		.renderPass = rpass,
		.framebuffer = frame->buffer,
		.renderArea = render_rect,
		.clearValueCount = ARRAY_SIZE(clear_colors),
		.pClearValues = clear_colors,
	};
	vkCmdBeginRenderPass(frame->cmds, &rbf, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdEndRenderPass(frame->cmds);
	return vkEndCommandBuffer(frame->cmds);
}

VkResult vkframe_init(struct vkframe *frame, const VkRenderPass rpass,
		      const struct vkrenderer *rdr, const VkImage image)
{
	frame->image = image;
	frame->size = rdr->srf_caps.currentExtent;
	const VkFormat format = rdr->srf_format.format;
	const VkDevice dev = rdr->device;
	VkResult err;
	if ((err = vkframe_init_view(frame, format, dev)) != VK_SUCCESS)
		return err;
	if ((err = vkframe_init_framebuffer(frame, rpass, dev)) != VK_SUCCESS)
		return err;
	if ((err = vkframe_alloc_cmds(frame, rdr->cmd_pool, dev)) != VK_SUCCESS)
		return err;
	return vkframe_record(frame, rpass);
}

void vkframe_destroy(const struct vkframe *frame, const VkDevice device)
{
	vkDestroyFramebuffer(device, frame->buffer, NULL);
	vkDestroyImageView(device, frame->view, NULL);
}
