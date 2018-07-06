/**
 * @file
 * Topdax application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "topdax/vkrenderer.h"
#include "topdax/topdax.h"

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

void topdax_startup(struct application *obj)
{
	struct topdax *app = container_of(obj, struct topdax, app);
	vulkan_initialize(&app->vk);
}

void topdax_shutdown(struct application *obj)
{
	struct topdax *app = container_of(obj, struct topdax, app);
	vkrenderer_terminate(app->rdr);
	vkDestroyInstance(app->vk, NULL);
}

void topdax_activate(struct application *obj)
{
	struct topdax *topdax = container_of(obj, struct topdax, app);
	struct topdax_window *main_window = topdax->main_window;
	main_window->app = &topdax->app;
	window_init(&main_window->win);
	/* TODO: Must be initialized using vulkan instance and window (after
	 * window creation) since window WILL enforce selection of supported
	 * device */
	vkrenderer_init(topdax->rdr);
}

void topdax_close_window(struct window *obj)
{
	struct topdax_window *win =
	    container_of(obj, struct topdax_window, win);
	application_quit(win->app);
}

/** Implementation of main Topdax window */
static const struct window_ops topdax_window_ops = {
	.close = topdax_close_window,
};

/** Implementation of Topdax application */
static const struct application_ops topdax_ops = {
	.startup = topdax_startup,
	.activate = topdax_activate,
	.shutdown = topdax_shutdown,
};

static const struct application_info topdax_info = {
	.version = PACKAGE_STRING,
	.bug_address = PACKAGE_BUGREPORT,
	.summary = "The program that renders triangle using Vulkan API",
};

/** Renderer instance */
static struct vkrenderer renderer;

/** Main topdax window */
static struct topdax_window main_window = {
	.win = {
		.width = 960,
		.height = 540,
		.caption = "Topdax",
		.ops = &topdax_window_ops,
		},
};

/** Topdax application instance */
static struct topdax app = {
	.rdr = &renderer,
	.app = {
		.ops = &topdax_ops,
		},
	.main_window = &main_window,
};

int topdax_run(int argc, char **argv)
{
	return application_run(&app.app, &topdax_info, argc, argv);
}
