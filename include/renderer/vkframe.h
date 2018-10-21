#ifndef RENDERER_VKFRAME_H
#define RENDERER_VKFRAME_H

struct vkrenderer;

/** Presentable frame in swapchain */
struct vkframe {
	/** Image in swapchain */
	VkImage image;
	/** View to image in swapchain */
	VkImageView view;
	/** Attached framebuffer*/
	VkFramebuffer framebuffer;
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
 * @param rdr Specifies renderer this frame belongs to
 * @param image Specifies swapchain image to init frame on
 * @returns VK_SUCCESS on success, or VkResult error otherwise
 */
VkResult vkframe_init(struct vkframe *frame, const struct vkrenderer *rdr,
		      const VkImage image);

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
