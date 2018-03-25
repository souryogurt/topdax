/**
 * @file
 * Test suite for glfw application
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>
#include <unistd.h>
#include "topdax/application.h"
#include <GLFW/glfw3.h>

GLFWAPI int glfwInit(void)
{
	return mock();
}

GLFWAPI void glfwPollEvents(void)
{
	mock();
}

GLFWAPI void glfwTerminate(void)
{
	mock();
}

static void mock_startup(struct application *obj)
{
	mock(obj);
}

static void mock_activate(struct application *obj)
{
	mock(obj);
	obj->is_running = 0;
}

static void mock_shutdown(struct application *obj)
{
	mock(obj);
}

static const struct application_ops test_ops = {
	.startup = mock_startup,
	.activate = mock_activate,
	.shutdown = mock_shutdown,
};

Ensure(app_calls_callbacks)
{
	char *argv[] = { "./topdax", NULL };
	struct application app = {
		.ops = &test_ops,
	};
	expect(glfwInit, will_return(GLFW_TRUE));
	expect(mock_startup, when(obj, is_equal_to(&app)));
	expect(mock_activate, when(obj, is_equal_to(&app)));
	expect(glfwPollEvents);
	expect(mock_shutdown, when(obj, is_equal_to(&app)));
	expect(glfwTerminate);
	int exit_code = application_run(&app, 1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_SUCCESS));
}

Ensure(app_exit_with_error_on_glfw_failure)
{
	char *argv[] = { "./topdax", NULL };
	struct application app = {
		.ops = &test_ops,
	};
	expect(glfwInit, will_return(GLFW_FALSE));
	never_expect(mock_startup);
	never_expect(mock_activate);
	never_expect(mock_shutdown);
	int exit_code = application_run(&app, 1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(app_exit_with_error_on_unknown_flag)
{
	char *argv[] = { "./topdax", "--unsupported", NULL };
	struct application app = {
		.ops = &test_ops,
	};
	/* Suppress stderr */
	close(2);
	never_expect(glfwInit);
	never_expect(mock_startup);
	never_expect(mock_activate);
	never_expect(mock_shutdown);
	int exit_code = application_run(&app, 2, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("GLFW");
	TestSuite *app = create_named_test_suite("Application");
	add_test(app, app_calls_callbacks);
	add_test(app, app_exit_with_error_on_glfw_failure);
	add_test(app, app_exit_with_error_on_unknown_flag);
	add_suite(suite, app);
	return run_test_suite(suite, create_text_reporter());
}
