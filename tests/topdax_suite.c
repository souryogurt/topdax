/**
 * @file
 * Test suite for topdax
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <check.h>
#include <signal.h>
#include <unistd.h>

START_TEST(test_first)
{
	ck_abort_msg("test that fails");
}
END_TEST

Suite *topdax_suite(void)
{
	Suite *s = suite_create("topdax");

	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_first);
	suite_add_tcase(s, tc_core);
	return s;
}

