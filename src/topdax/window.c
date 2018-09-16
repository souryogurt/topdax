/**
 * @file
 * Topdax main window
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <application/utils.h>
#include <application/runloop.h>
#include "window.h"
#include "topdax.h"

void topdax_close_window(struct window_handler *obj, struct window *win)
{
	UNUSED(win);
	struct topdax_window *window =
	    container_of(obj, struct topdax_window, handler);
	struct runloop *mainloop = window->app->mainloop;
	mainloop->ops->quit(mainloop);
}

/** Implementation of main Topdax window */
static const struct window_handler_ops topdax_window_ops = {
	.close = topdax_close_window,
};

void topdax_window_init(struct topdax_window *win, struct topdax *app)
{
	win->handler.ops = &topdax_window_ops;
	win->app = app;

#ifdef TOPDAX_USE_PLATFORM_GLFW
	glfw_window_init(&win->window, 960, 540, "Topdax", &win->handler);
#endif
	/* TODO: Must be initialized using vulkan instance and window (after
	 * window creation) since window WILL enforce selection of supported
	 * device */
	vkrenderer_init(&win->renderer);
}

void topdax_window_destroy(struct topdax_window *win)
{
	vkrenderer_terminate(&win->renderer);
}
