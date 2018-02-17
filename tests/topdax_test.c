#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_that_success(void **state)
{
	(void)state;
	assert_int_equal(2, 2);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_that_success),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
