#ifndef RENDERER_VKFRAME_H
#define RENDERER_VKFRAME_H

#include <vulkan/vulkan_core.h>

struct vkrenderer;

/** Presentable frame in swapchain */
struct vkframe {
	/** Attached buffer*/
	VkFramebuffer buffer;
	/** View to image in swapchain */
	VkImageView view;
	/** Image in swapchain */
	VkImage image;
	/** Frame dimensions */
	VkExtent2D size;
	/** Primary command buffer */
	VkCommandBuffer cmds;
};
#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Initializes swapchain frame
 * @param frame Specifies frame to initialize
 * @param rpass Specifies render pass the frame will be compatible with
 * @param rdr Specifies renderer this frame belongs to
 * @param image Specifies swapchain image to init frame on
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
VkResult vkframe_init(struct vkframe *frame, const VkRenderPass rpass,
		      const struct vkrenderer *rdr, const VkImage image);

/**
 * Destroys frame resources
 * @param frame Specifies frame to destroy
 * @param device Specifies device instance this frame belongs to
 */
void vkframe_destroy(const struct vkframe *frame, const VkDevice device);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
