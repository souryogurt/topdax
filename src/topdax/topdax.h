#ifndef TOPDAX_H
#define TOPDAX_H
#include <topdax/application.h>

#ifdef TOPDAX_USE_PLATFORM_GLFW
#include <GLFW/window.h>
#endif

#include <renderer/vkrenderer.h>

struct topdax;

/** Main topdax window */
struct topdax_window {
	/** Implements window handler */
	struct window_handler handler;

	/** Pointer to application */
	struct topdax *app;

	/** Window implementation */
#ifdef TOPDAX_USE_PLATFORM_GLFW
	struct glfw_window window;
#endif
	/** Renderer implementation */
	struct vkrenderer renderer;
};

/** Topdax Application */
struct topdax {
	/** Vulkan instance */
	VkInstance vk;
	/** Pointer to mainloop */
	struct runloop *mainloop;

	/** Main window */
	struct topdax_window window;
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
 */
void topdax_window_init(struct topdax_window *win, struct topdax *app);

/**
 * Destroy main window
 * @param win Specifies pointer to window to initialize
 */
void topdax_window_destroy(struct topdax_window *win);

/**
 * Close application window
 * @param obj Specifies pointer to topdax window
 */
void topdax_close_window(struct window_handler *obj, struct window *win);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
