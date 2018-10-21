/**
 * @file
 * Topdax main window
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "window.h"
#include <application/utils.h>
#include <renderer/vkrenderer.h>
#include "topdax.h"

/**
 * Call window's close callback
 * @param glfw_window Specifies GLFW window that must be closed
 */
static void topdax_window_close(GLFWwindow * glfw_window)
{
	UNUSED(glfw_window);
	application_quit();
}

int topdax_window_init(struct topdax_window *win, VkInstance vk)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	win->id = glfwCreateWindow(960, 540, "Topdax", NULL, NULL);
	if (win->id == NULL) {
		return 1;
	}
	glfwSetWindowUserPointer(win->id, win);
	glfwSetWindowCloseCallback(win->id, topdax_window_close);
	if (glfwCreateWindowSurface(vk, win->id, NULL, &win->surface) !=
	    VK_SUCCESS) {
		return 1;
	}
	vkrenderer_init(&win->renderer, vk, win->surface);
	vkrenderer_render(&win->renderer);
	return 0;
}

void topdax_window_destroy(struct topdax_window *win)
{
	vkrenderer_terminate(&win->renderer);
	vkDestroySurfaceKHR(win->vk, win->surface, NULL);
}
