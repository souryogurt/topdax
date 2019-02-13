/**
 * @file
 * Topdax main window
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "window.h"
#include <renderer/vkrenderer.h>
#include "topdax.h"

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
	vkrenderer_init(&win->renderer, instance, win->surface);
	return 0;
}

void topdax_window_destroy(struct topdax_window *win)
{
	vkrenderer_terminate(&win->renderer);
	vkDestroySurfaceKHR(win->vk, win->surface, NULL);
}
