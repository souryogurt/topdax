#ifndef RENDERER_VKSWAPCHAIN_H
#define RENDERER_VKSWAPCHAIN_H

#include <vulkan/vulkan_core.h>
#include <renderer/vkframe.h>

struct vkrenderer;

/** Vulkan Swapchain */
struct vkswapchain {
	/** Swapchain */
	VkSwapchainKHR swapchain;
	/** Rendered frames */
	struct vkframe frames[16];
	/** Number of frames in swapchain */
	size_t nframes;
	/** Semaphore signaled when image is acquired from swapchain */
	VkSemaphore acquire_sem;
	/** Semaphore signaled when image is rendered */
	VkSemaphore render_sem;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Initialize swapchain struct
 * @param swc Specifies pointer to vkswapchain struct to initialize
 * @param rdr Specifies renderer to get parameters from
 * @param old Specifies handle of old swapchain, or VK_NULL_HANDLE
 * @returns zero on sucess, or non-zero otherwise
 */
int vkswapchain_init(struct vkswapchain *swc, const struct vkrenderer *rdr,
		     const VkSwapchainKHR old_swc);

/**
 * Render to surface associated with renderer
 * @param swc Specifies pointer to swapchain used as target
 * @param rdr Specifies pointer to renderer
 * @returns VK_SUCCESS on success, or VkError error otherwise
 */
VkResult vkswapchain_render(const struct vkswapchain *swc,
			    const struct vkrenderer *rdr);
/**
 * Terminate swapchain
 * @param swc Specifies pointer to vkswapchain to terminate
 * @param dev Spwcifies Vulkan device to remove swapchain from
 */
void vkswapchain_terminate(const struct vkswapchain *swc, VkDevice dev);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
