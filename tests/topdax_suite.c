/**
 * @file
 * Test suite for topdax
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include "topdax/topdax.h"

START_TEST(test_verbose_flag)
{
	char *argv[] = { "./topdax", "--verbose", NULL };
	ck_assert_int_eq(topdax_run(2, &argv[0]), EXIT_SUCCESS);
}
END_TEST

START_TEST(test_unsupported_flag)
{
	char *argv[] = { "./topdax", "--unsupported", NULL };
	/* Do not print anything on assert */
	close(2);
	topdax_run(2, &argv[0]);
}
END_TEST

int main(void)
{
	Suite *suite = suite_create("topdax");

	TCase *tc_core = tcase_create("Core");
	tcase_add_exit_test(tc_core, test_unsupported_flag, 64);
	tcase_add_test(tc_core, test_verbose_flag);
	suite_add_tcase(suite, tc_core);

	SRunner *sr = srunner_create(suite);
	srunner_run_all(sr, CK_ENV);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
