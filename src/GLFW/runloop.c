/**
 * @file
 * GLFW implementation of runloop
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <argp.h>
#include <stdlib.h>

#include "GLFW/runloop.h"
#include <GLFW/glfw3.h>

/** Program version */
const char *argp_program_version;

/** Bug address */
const char *argp_program_bug_address;

/** Arguments parser */
static struct argp argp;

static void glfw_quit(struct runloop *obj)
{
	struct glfw_runloop *loop =
	    container_of(obj, struct glfw_runloop, loop);
	loop->must_quit++;
}

static const struct runloop_ops glfw_ops = {
	.quit = &glfw_quit
};

int glfw_runloop_run(struct glfw_runloop *loop,
		     const struct application_ops *app,
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

	loop->loop.ops = &glfw_ops;
	loop->must_quit = 0;
	if (app->startup)
		app->startup(&loop->loop);
	if (app->activate)
		app->activate();
	do {
		glfwWaitEvents();
	} while (!loop->must_quit);

	if (app->shutdown)
		app->shutdown();
	glfwTerminate();
	return EXIT_SUCCESS;
}
