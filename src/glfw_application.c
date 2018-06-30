/**
 * @file
 * GLFW implementation of application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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
	struct window *win;
	win = (struct window *)glfwGetWindowUserPointer(glfw_window);
	if (win->ops->close)
		win->ops->close(win);

}

int window_init(struct window *win)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	win->id = glfwCreateWindow(win->width, win->height, win->caption,
				   NULL, NULL);
	if (win->id) {
		glfwSetWindowUserPointer(win->id, win);
		glfwSetWindowCloseCallback(win->id, close_callback);
		return 0;
	}
	return 1;
}

void window_close(struct window *win)
{
	close_callback(win->id);
}

int application_run(struct application *app, int argc, char **argv)
{
	const struct application_info *app_info = app->info;
	if (app_info) {
		argp_program_version = app_info->version;
		argp_program_bug_address = app_info->bug_address;
		argp.doc = app_info->summary;
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
