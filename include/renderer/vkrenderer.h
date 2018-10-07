#ifndef TOPDAX_VKRENDERER_H
#define TOPDAX_VKRENDERER_H

#include <vulkan/vulkan.h>

/** Vulkan Renderer Instance */
struct vkrenderer {
	/** Target surface presenting rendered image */
	VkSurfaceKHR srf;
	/** Physical device */
	VkPhysicalDevice phy;
	/** Enabled device features */
	VkPhysicalDeviceFeatures features;
	/** Enabled logical device extensions */
	const char *const *extensions;
	/** Number of enabled logical device extensions */
	uint32_t nextensions;
	/** Queue family index that supports graphics operations */
	uint32_t graphic;
	/** Queue family index that supports presentation */
	uint32_t present;
	/** Vulkan device */
	VkDevice device;
	/** Graphics Queue */
	VkQueue graphics_queue;
	/** Presenting Queue */
	VkQueue present_queue;
	/** Surface capabilities */
	VkSurfaceCapabilitiesKHR srf_caps;
	/** Surface format */
	VkSurfaceFormatKHR srf_format;
	/** Present Mode */
	VkPresentModeKHR srf_mode;
	/** Swapchain */
	VkSwapchainKHR swapchain;
	/** Rendered frames */
	VkImage frames[16];
	/** Frame views */
	VkImageView frame_views[16];
	/** Framebuffers */
	VkFramebuffer framebuffers[16];
	/** Number of frames in swapchain */
	uint32_t nframes;
	/** Render pass */
	VkRenderPass renderpass;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Initializes Vulkan renderer instance
 * @param rdr Specifies pointer to renderer to initialize
 * @param instance Specifies Vulkan instance
 * @param surface Specifies Window surface which will present result
 * @returns zero on success, and non-zero otherwise
 */
int vkrenderer_init(struct vkrenderer *rdr, VkInstance instance,
		    VkSurfaceKHR surface);

/**
 * Terminates Vulkan renderer instance
 * @param rdr Specifies pointer to renderer to terminate
 */
void vkrenderer_terminate(struct vkrenderer *rdr);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
