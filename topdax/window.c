/**
 * @file
 * Topdax main window
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "window.h"

int topdax_window_init(struct topdax_window *win, VkInstance instance)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	win->id = glfwCreateWindow(960, 540, "Topdax", NULL, NULL);
	if (win->id == NULL) {
		return 1;
	}
	win->vk = instance;
	glfwSetWindowUserPointer(win->id, win);
	if (glfwCreateWindowSurface(instance, win->id, NULL, &win->surface) !=
	    VK_SUCCESS) {
		return 1;
	}
	return 0;
}

void topdax_window_destroy(const struct topdax_window *win)
{
	vkDestroySurfaceKHR(win->vk, win->surface, NULL);
	glfwDestroyWindow(win->id);
}
