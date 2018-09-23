#ifndef TOPDAX_VKRENDERER_H
#define TOPDAX_VKRENDERER_H

#include <vulkan/vulkan.h>

/** Vulkan Renderer Instance */
struct vkrenderer {
	/** Vulkan instance */
	VkInstance instance;
	/** Vulkan device */
	VkDevice device;
	/** Graphics Queue */
	VkQueue graphics_queue;
	/** Predenting Queue */
	VkQueue present_queue;
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
