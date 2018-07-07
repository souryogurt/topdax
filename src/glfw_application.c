/**
 * @file
 * GLFW implementation of application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <argp.h>
#include <stdlib.h>

#include "topdax/application.h"
#include <GLFW/glfw3.h>

/** Program version */
const char *argp_program_version;

/** Bug address */
const char *argp_program_bug_address;

/** Arguments parser */
static struct argp argp;

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

void window_close(struct window *obj)
{
	struct glfw_window *win = container_of(obj, struct glfw_window, win);
	close_callback(win->id);
}

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

int application_run(struct application *app,
		    const struct application_info *info, int argc, char **argv)
{
	if (info) {
		argp_program_version = info->version;
		argp_program_bug_address = info->bug_address;
		argp.doc = info->summary;
	}
	if (argp_parse(&argp, argc, argv, 0, NULL, NULL))
		return EXIT_FAILURE;
	if (!glfwInit())
		return EXIT_FAILURE;
	const struct application_ops *ops = app->ops;
	if (ops->startup)
		ops->startup(app);
	if (ops->activate)
		ops->activate(app);
	do {
		glfwWaitEvents();
	} while (!app->must_quit);

	if (ops->shutdown)
		ops->shutdown(app);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void application_quit(struct application *app)
{
	app->must_quit++;
}
