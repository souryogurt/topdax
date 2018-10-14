/**
 * @file
 * GLFW implementation of runloop
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <argp.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <application/utils.h>
#include "topdax.h"

/** Runloop implementation using GLFW API */
struct glfw_runloop {
	/** Implements runloop interface */
	struct runloop loop;
	/** zero if application is running, non-zero otherwise */
	int must_quit;
};

/** Version string */
const char *const g_app_version;

/** Name and email of person responsible for issues */
const char *const g_app_bug_address;

/** Application description */
const char *const g_app_description;

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

int topdax_main(int argc, char **argv)
{
	argp_program_version = g_app_version;
	argp_program_bug_address = g_app_bug_address;
	argp.doc = g_app_description;
	if (argp_parse(&argp, argc, argv, 0, NULL, NULL))
		return EXIT_FAILURE;

	struct glfw_runloop glfw_loop = {
		.loop = {
			 .ops = &glfw_ops,
			 },
		.must_quit = 0,
	};
	if (application_startup(&glfw_loop.loop)) {
		return EXIT_FAILURE;
	}
	do {
		glfwWaitEvents();
	} while (!glfw_loop.must_quit);
	application_shutdown();
	return EXIT_SUCCESS;
}
