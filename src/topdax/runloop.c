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
#include "topdax.h"

/** zero if application is running, non-zero otherwise */
static int g_app_must_quit;

/** Version string */
const char *const g_app_version;

/** Name and email of person responsible for issues */
const char *const g_app_bug_address;

/** Application description */
const char *const g_app_description;

/** Arguments parser */
static struct argp argp;

void topdax_quit(void)
{
	g_app_must_quit++;
}

int topdax_main(int argc, char **argv)
{
	argp_program_version = g_app_version;
	argp_program_bug_address = g_app_bug_address;
	argp.doc = g_app_description;
	if (argp_parse(&argp, argc, argv, 0, NULL, NULL))
		return EXIT_FAILURE;

	if (application_startup()) {
		return EXIT_FAILURE;
	}
	do {
		glfwWaitEvents();
	} while (!g_app_must_quit);
	application_shutdown();
	return EXIT_SUCCESS;
}
