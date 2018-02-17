/**
 * @file
 * Topdax entrypoint
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>

#include "topdax/topdax.h"

/** Program version */
const char *argp_program_version = PACKAGE_STRING;

/** Bug address */
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

/** Program description */
static const char description[] =
    "The program that renders triangle using Vulkan API";

/** Option flags and variables */
static const struct argp_option options[] = {
	{"verbose", 'v', 0, 0, "Produce verbose output", 0},
	{0}
};

/** Parse a single option */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	UNUSED(arg);
	struct arguments *arguments = state->input;

	switch (key) {
	case 'v':
		arguments->verbose = true;
		break;
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
	.doc = description,
};

/** Entry point */
int main(int argc, char **argv)
{
	struct arguments args = {
		.verbose = false,
	};
	error_t parse_err = argp_parse(&argp, argc, argv, 0, NULL, &args);
	if (parse_err)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
