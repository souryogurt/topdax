/**
 * @file
 * Topdax application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef TOPDAX_USE_PLATFORM_GLFW
#include <GLFW/window.h>
#include <GLFW/runloop.h>
#endif

#include "topdax/vkrenderer.h"
#include "topdax/topdax.h"

#include <vulkan/vulkan.h>

/** Topdax Application */
struct topdax {
	/** Vulkan instance */
	VkInstance vk;
	/** Pointer to renderer */
	struct vkrenderer *rdr;
	/** Implements window handler */
	struct window_handler handler;
	/** Pointer to mainloop */
	struct runloop *mainloop;
};

static int vulkan_initialize(VkInstance * vk)
{
	const VkInstanceCreateInfo create_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = NULL,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = 0,
		.ppEnabledExtensionNames = NULL,
	};
	VkResult result = vkCreateInstance(&create_info, NULL, vk);
	return result != VK_SUCCESS;
}

/** Renderer instance */
static struct vkrenderer renderer;

/** Topdax application instance */
static struct topdax app;

void topdax_close_window(struct window_handler *obj, struct window *win)
{
	UNUSED(obj);
	UNUSED(win);
	app.mainloop->ops->quit(app.mainloop);
}

/** Implementation of main Topdax window */
static const struct window_handler_ops topdax_window_ops = {
	.close = topdax_close_window,
};

/**
 * Starts up application components
 */
static void topdax_startup(struct runloop *loop)
{
	app.rdr = &renderer;
	app.handler.ops = &topdax_window_ops;
	app.mainloop = loop;
	vulkan_initialize(&app.vk);
}

/**
 * Shuts down application components
 */
static void topdax_shutdown(void)
{
	vkrenderer_terminate(app.rdr);
	vkDestroyInstance(app.vk, NULL);
}

/** Main topdax window */
static struct glfw_window main_window;

/**
 * Activate application
 */
static void topdax_activate(void)
{
	glfw_window_init(&main_window, 960, 540, "Topdax", &app.handler);
	/* TODO: Must be initialized using vulkan instance and window (after
	 * window creation) since window WILL enforce selection of supported
	 * device */
	vkrenderer_init(app.rdr);
}

const struct application_ops topdax_ops = {
	.startup = topdax_startup,
	.activate = topdax_activate,
	.shutdown = topdax_shutdown,
};

static const struct application_info topdax_info = {
	.version = PACKAGE_STRING,
	.bug_address = PACKAGE_BUGREPORT,
	.summary = "The program that renders triangle using Vulkan API",
};

int topdax_run(int argc, char **argv)
{
	struct glfw_runloop g_mainloop;

	return glfw_runloop_run(&g_mainloop, &topdax_ops, &topdax_info, argc,
				argv);
}
