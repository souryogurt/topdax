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
 * @param rdr Specifies renderer of this frame
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkframe_init_framebuffer(struct vkframe *frame,
					 const struct vkrenderer *rdr)
{

	VkImageView attachments[] = { frame->view };
	VkFramebufferCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.renderPass = rdr->swcs[rdr->swc_index].renderpass,
		.attachmentCount = ARRAY_SIZE(attachments),
		.pAttachments = attachments,
		.width = rdr->srf_caps.currentExtent.width,
		.height = rdr->srf_caps.currentExtent.height,
		.layers = 1,
	};
	return vkCreateFramebuffer(rdr->device, &info, NULL,
				   &frame->framebuffer);
}

/**
 * Initializes image view on frame's image
 * @param frame Specifies frame to initialize view for
 * @param rdr Specifies renderer of this frame
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkframe_init_view(struct vkframe *frame,
				  const struct vkrenderer *rdr)
{
	VkImageViewCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.image = frame->image,
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
	return vkCreateImageView(rdr->device, &info, NULL, &frame->view);
}

/**
 * Allocates primary command bufffer for frame
 * @param frame Specifies frame to allocate primary command buffer for
 * @param rdr Specifies renderer of this frame
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
static VkResult vkframe_alloc_cmds(struct vkframe *frame,
				   const struct vkrenderer *rdr)
{
	VkCommandBufferAllocateInfo info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext = NULL,
		.commandPool = rdr->cmd_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	return vkAllocateCommandBuffers(rdr->device, &info, &frame->cmds);
}

/**
 * Records commands to frame
 * @param frame Specifies the frame to recoord commands for
 * @param rdr Specifies renderer that will render this frame
 * @returns VK_SUCCESS on success, or error otherwise
 */
static VkResult vkframe_record(const struct vkframe *frame,
			       const struct vkrenderer *rdr)
{
	VkCommandBufferBeginInfo begin_info = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = NULL,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = NULL,
	};
	VkResult result;
	result = vkBeginCommandBuffer(frame->cmds, &begin_info);
	if (result != VK_SUCCESS)
		return result;

	VkClearValue clear_colors[] = {
		{
		 .color = {
			   .float32 = {1.0F, 1.0F, 1.0F, 1.0F}
			   }
		 }
	};
	VkRenderPassBeginInfo rbf = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.pNext = NULL,
		.renderPass = rdr->swcs[rdr->swc_index].renderpass,
		.framebuffer = frame->framebuffer,
		.renderArea = {
			       .offset = {0, 0},
			       .extent = rdr->srf_caps.currentExtent,
			       },
		.clearValueCount = ARRAY_SIZE(clear_colors),
		.pClearValues = clear_colors,
	};
	vkCmdBeginRenderPass(frame->cmds, &rbf, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdEndRenderPass(frame->cmds);
	return vkEndCommandBuffer(frame->cmds);
}

VkResult vkframe_init(struct vkframe *frame, const struct vkrenderer *rdr,
		      const VkImage image)
{
	frame->image = image;
	VkResult result;
	if ((result = vkframe_init_view(frame, rdr)) != VK_SUCCESS)
		return result;
	if ((result = vkframe_init_framebuffer(frame, rdr)) != VK_SUCCESS)
		return result;
	if ((result = vkframe_alloc_cmds(frame, rdr)) != VK_SUCCESS)
		return result;
	if ((result = vkframe_record(frame, rdr)) != VK_SUCCESS)
		return result;
	return result;
}

void vkframe_destroy(const struct vkframe *frame, const VkDevice device)
{
	vkDestroyFramebuffer(device, frame->framebuffer, NULL);
	vkDestroyImageView(device, frame->view, NULL);
}
