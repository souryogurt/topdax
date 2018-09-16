#ifndef TOPDAX_WINDOW_H
#define TOPDAX_WINDOW_H

#ifdef TOPDAX_USE_PLATFORM_GLFW
#include <GLFW/glfw3.h>
#endif
#include <renderer/vkrenderer.h>

struct topdax;

/** Main topdax window */
struct topdax_window {
	/** Pointer to application */
	struct topdax *app;

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
 * @param app Specifies pointer to application this window belongs to
 * @return zero on success, non-zero otherwise
 */
int topdax_window_init(struct topdax_window *win, struct topdax *app);

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
