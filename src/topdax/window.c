/**
 * @file
 * Topdax main window
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <application/runloop.h>
#include "window.h"
#include "topdax.h"

/**
 * Call window's close callback
 * @param glfw_window Specifies GLFW window that must be closed
 */
static void topdax_window_close(GLFWwindow * glfw_window)
{
	struct topdax_window *win;
	win = (struct topdax_window *)glfwGetWindowUserPointer(glfw_window);
	struct runloop *mainloop = win->app->mainloop;
	mainloop->ops->quit(mainloop);
}

int topdax_window_init(struct topdax_window *win, struct topdax *app)
{
	win->app = app;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	win->id = glfwCreateWindow(960, 540, "Topdax", NULL, NULL);
	if (win->id) {
		glfwSetWindowUserPointer(win->id, win);
		glfwSetWindowCloseCallback(win->id, topdax_window_close);
		/* TODO: Must be initialized using vulkan instance and window
		 * surface (after window creation) since window surface WILL
		 * enforce selection of supported device */
		vkrenderer_init(&win->renderer);
		return 0;
	}
	return 1;
}

void topdax_window_destroy(struct topdax_window *win)
{
	vkrenderer_terminate(&win->renderer);
}
