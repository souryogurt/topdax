#ifndef RENDERER_VKRENDERER_H
#define RENDERER_VKRENDERER_H

#include <renderer/vkswapchain.h>

/** Returns array size */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

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
	/** Command pool */
	VkCommandPool cmd_pool;
	/** Render pass */
	VkRenderPass rpass;
	/** An array of swapchains */
	struct vkswapchain swcs[2];
	/** Current swapchain */
	size_t swc_index;
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
		    const VkSurfaceKHR surface);

/**
 * Renders an image to attached surface
 * @param rdr Specifies pointer to renderer
 * @returns zero on success, and non-zero otherwise
 */
int vkrenderer_render(struct vkrenderer *rdr);

/**
 * Terminates Vulkan renderer instance
 * @param rdr Specifies pointer to renderer to terminate
 */
void vkrenderer_terminate(const struct vkrenderer *rdr);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
