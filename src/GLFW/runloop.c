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

/** Version string */
const char *application_version;

/** Name and email of person responsible for issues */
const char *application_bug_address;

/** Application description */
const char *application_description;

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

int glfw_runloop_run(struct glfw_runloop *loop, int argc, char **argv)
{
	argp_program_version = application_version;
	argp_program_bug_address = application_bug_address;
	argp.doc = application_description;
	if (argp_parse(&argp, argc, argv, 0, NULL, NULL))
		return EXIT_FAILURE;
	if (!glfwInit())
		return EXIT_FAILURE;

	loop->loop.ops = &glfw_ops;
	loop->must_quit = 0;
	application_startup(&loop->loop);
	application_activate();
	do {
		glfwWaitEvents();
	} while (!loop->must_quit);
	application_shutdown();
	glfwTerminate();
	return EXIT_SUCCESS;
}
