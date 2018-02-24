/**
 * @file
 * Test suite for topdax
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cgreen/cgreen.h>
#include <stdlib.h>
#include <unistd.h>
#include "topdax/topdax.h"

Describe(topdax);
BeforeEach(topdax)
{
}

AfterEach(topdax)
{
}

Ensure(topdax, verbose_flag)
{
	char *argv[] = { "./topdax", "--verbose", NULL };
	int exit_code = topdax_run(2, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_SUCCESS));
}

Ensure(topdax, unsupported_flag)
{
	char *argv[] = { "./topdax", "--unsupported", NULL };
	/** Suppress stderr */
	close(2);
	int exit_code = topdax_run(2, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_test_suite();
	add_test_with_context(suite, topdax, unsupported_flag);
	add_test_with_context(suite, topdax, verbose_flag);
	return run_test_suite(suite, create_text_reporter());
}
