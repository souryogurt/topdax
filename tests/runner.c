/**
 * @file
 * Test runner
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <check.h>
#include <stdlib.h>

extern Suite *topdax_suite(void);

int main(void)
{
	SRunner *sr = srunner_create(topdax_suite());
	srunner_run_all(sr, CK_ENV);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
