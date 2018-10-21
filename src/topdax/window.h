#ifndef TOPDAX_WINDOW_H
#define TOPDAX_WINDOW_H

#include <renderer/vkrenderer.h>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

/** Main topdax window */
struct topdax_window {
	/** Vulkan instance used to render this window */
	VkInstance vk;

	/** Instance of GLFW window */
	GLFWwindow *id;

	/** Vulkan surface of this window */
	VkSurfaceKHR surface;

	/** Renderer implementation */
	struct vkrenderer renderer;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Initialize main window
 * @param win Specifies pointer to window to initialize
 * @param instance Specifies Vulkan instance where this window will be rendered
 * @return zero on success, non-zero otherwise
 */
int topdax_window_init(struct topdax_window *win, VkInstance instance);

/**
 * Destroy main window
 * @param win Specifies pointer to window to initialize
 */
void topdax_window_destroy(struct topdax_window *win);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
