/**
 * @file
 * GLFW implementation of application
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <argp.h>
#include <stdlib.h>

#include "topdax/application.h"
#include <GLFW/glfw3.h>

/** Program version */
const char *argp_program_version = PACKAGE_STRING;

/** Bug address */
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

/** Option flags and variables */
static const struct argp_option options[] = {
	{0}
};

/** Parse a single option */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	(void)(arg);
	(void)(state);
	switch (key) {
	case ARGP_KEY_END:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/** Arguments parser */
static const struct argp argp = {
	.options = options,
	.parser = parse_opt,
	/*
	 * TODO: Define this in subclass 
	 */
	.doc = "The program that renders triangle using Vulkan API",
};

int application_run(struct application *app, int argc, char **argv)
{
	if (argp_parse(&argp, argc, argv, ARGP_NO_EXIT, NULL, NULL))
		return EXIT_FAILURE;
	if (!glfwInit())
		return EXIT_FAILURE;
	const struct application_ops *ops = app->ops;
	if (ops->startup)
		ops->startup(app);
	app->is_running = 1;
	if (ops->activate)
		ops->activate(app);
	do {
		glfwPollEvents();
	} while (app->is_running);

	if (ops->shutdown)
		ops->shutdown(app);
	glfwTerminate();
	return EXIT_SUCCESS;
}
