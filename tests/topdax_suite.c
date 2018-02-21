/**
 * @file
 * Test suite for topdax
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <check.h>
#include <stdlib.h>

START_TEST(test_first)
{
	if (0) {
		ck_abort_msg("test that fails");
	}
}
END_TEST

int main(void)
{
	Suite *suite = suite_create("topdax");

	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_first);
	suite_add_tcase(suite, tc_core);

	SRunner *sr = srunner_create(suite);
	srunner_run_all(sr, CK_ENV);
	int number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
