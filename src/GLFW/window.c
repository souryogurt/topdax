/**
 * @file
 * GLFW implementation of window
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <GLFW/window.h>

/**
 * Call window's close callback
 * @param glfw_window Specifies GLFW window that must be closed
 */
static void close_callback(GLFWwindow * glfw_window)
{
	struct glfw_window *win;
	win = (struct glfw_window *)glfwGetWindowUserPointer(glfw_window);
	struct window_handler *wh = win->handler;
	if (wh->ops->close)
		wh->ops->close(wh, &win->win);

}

/**
 * Requests that the window is closed, similar to what happens when a window
 * manager close button is clicked.
 * @param obj Specifies pointer to window to close
 */
static void window_close(struct window *obj)
{
	struct glfw_window *win = container_of(obj, struct glfw_window, win);
	close_callback(win->id);
}

/** Implementation of window interface */
static const struct window_ops glfw_window_ops = {
	.close = window_close,
};

int glfw_window_init(struct glfw_window *win, int width, int height,
		     const char *caption, struct window_handler *wh)
{
	assert(wh);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	win->id = glfwCreateWindow(width, height, caption, NULL, NULL);
	if (win->id) {
		win->win.ops = &glfw_window_ops;
		win->handler = wh;
		glfwSetWindowUserPointer(win->id, win);
		glfwSetWindowCloseCallback(win->id, close_callback);
		return 0;
	}
	return 1;
}
