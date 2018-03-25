/**
 * @file
 * Topdax entrypoint
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <argp.h>
#include <stdlib.h>

#include "topdax/topdax.h"
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
	UNUSED(arg);
	UNUSED(state);
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
	.doc = "The program that renders triangle using Vulkan API",
};

int topdax_run(int argc, char **argv)
{
	error_t err = argp_parse(&argp, argc, argv, ARGP_NO_EXIT, NULL, NULL);
	if (err)
		return EXIT_FAILURE;
	if (!glfwInit())
		return EXIT_FAILURE;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *window = glfwCreateWindow(960, 540, "Topdax", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return EXIT_FAILURE;
	}
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}
