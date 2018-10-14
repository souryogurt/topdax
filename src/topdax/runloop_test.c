/**
 * @file
 * Test suite for glfw application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include <argp.h>

#include "topdax.h"

/** Version string */
const char *const g_app_version = "TestApplicationVersionString";

/** Name and email of person responsible for issues */
const char *const g_app_bug_address = "TestApplicationBugAddress";

/** Application description */
const char *const g_app_description = "TestApplicationSummaryString";

GLFWAPI void glfwWaitEvents(void)
{
	mock();
}

error_t __wrap_argp_parse(const struct argp *__restrict __argp,
			  int __argc, char **__restrict __argv,
			  unsigned __flags, int *__restrict __arg_index,
			  void *__restrict __input)
{
	const char *argp_doc = __argp->doc;
	const char *argp_version = argp_program_version;
	const char *argp_bug_address = argp_program_bug_address;
	return (error_t) mock(__argp, __argc, __argv, __flags, __arg_index,
			      __input, argp_doc, argp_version,
			      argp_bug_address);
}

int application_startup(struct runloop *loop)
{
	int result = mock(loop);
	loop->ops->quit(loop);
	return result;
}

void application_shutdown()
{
	mock();
}

Ensure(app_calls_callbacks)
{
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(application_startup);
	expect(glfwWaitEvents);
	expect(application_shutdown);
	int exit_code = topdax_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_SUCCESS));
}

Ensure(app_exit_with_error_on_startup_fail)
{
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(application_startup, will_return(1));
	never_expect(application_shutdown);
	int exit_code = topdax_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(app_accepts_help_argument_when_summary_provided)
{
	char *argv[] = { "./topdax", "--help", NULL };
	expect(__wrap_argp_parse, will_return(EXIT_FAILURE),
	       when(argp_doc, is_equal_to_string(g_app_description)),
	       when(argp_version, is_equal_to_string(g_app_version)),
	       when(argp_bug_address, is_equal_to_string(g_app_bug_address)));
	int exit_code = topdax_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("GLFW Runloop");
	add_test(suite, app_calls_callbacks);
	add_test(suite, app_exit_with_error_on_startup_fail);
	add_test(suite, app_accepts_help_argument_when_summary_provided);
	return run_test_suite(suite, create_text_reporter());
}
