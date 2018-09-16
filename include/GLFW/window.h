#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <application/window.h>
#include <GLFW/glfw3.h>

/** Window implementation using GLFW API */
struct glfw_window {
	/** Implements window interface */
	struct window win;
	/** Instance of GLFW window */
	GLFWwindow *id;
	/** Pointer to window handler implementation */
	struct window_handler *handler;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Initializes and shows top-level window using GLFW
 * @param win Specifies pointer to glfw_window to initialize
 * @param width Specifies initial width of window
 * @param height Specifies initial height of window
 * @param caption Specifies initial caption of window
 * @param winh Specifies pointer to window handler that will respond to events
 * @return zero on success, non-zero otherwise
 */
int glfw_window_init(struct glfw_window *win, int width, int height,
		     const char *caption, struct window_handler *winh);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
