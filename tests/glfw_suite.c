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
#include <argp.h>

GLFWAPI int glfwInit(void)
{
	return mock();
}

GLFWAPI void glfwWaitEvents(void)
{
	mock();
}

GLFWAPI void glfwTerminate(void)
{
	mock();
}

GLFWAPI void glfwWindowHint(int hint, int value)
{
	mock(hint, value);
}

GLFWAPI GLFWwindow *glfwCreateWindow(int width, int height, const char *title,
				     GLFWmonitor * monitor, GLFWwindow * share)
{
	return (GLFWwindow *) mock(width, height, title, monitor, share);
}

GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow * window,
						      GLFWwindowclosefun cbfun)
{
	return (GLFWwindowclosefun) mock(window, cbfun);
}

GLFWAPI void glfwSetWindowUserPointer(GLFWwindow * window, void *pointer)
{
	mock(window, pointer);
}

GLFWAPI void *glfwGetWindowUserPointer(GLFWwindow * window)
{
	return (void *)mock(window);
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

static void mock_startup(struct application *obj)
{
	mock(obj);
}

static void mock_activate(struct application *obj)
{
	mock(obj);
	obj->must_quit = 1;
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
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(GLFW_TRUE));
	expect(mock_startup, when(obj, is_equal_to(&app)));
	expect(mock_activate, when(obj, is_equal_to(&app)));
	expect(glfwWaitEvents);
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
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(GLFW_FALSE));
	never_expect(mock_startup);
	never_expect(mock_activate);
	never_expect(mock_shutdown);
	int exit_code = application_run(&app, 1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(app_accepts_help_argument_when_summary_provided)
{
	char *argv[] = { "./topdax", "--help", NULL };
	const struct application_info info = {
		.summary = "TestApplicationSummaryString",
		.bug_address = "TestApplicationBugAddress",
		.version = "TestApplicationVersionString",
	};
	struct application app = {
		.ops = &test_ops,
		.info = &info,
	};
	expect(__wrap_argp_parse, will_return(EXIT_FAILURE),
	       when(argp_doc, is_equal_to_string(info.summary)),
	       when(argp_version, is_equal_to_string(info.version)),
	       when(argp_bug_address, is_equal_to_string(info.bug_address)));
	int exit_code = application_run(&app, 1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(app_quit_ends_the_main_loop)
{
	char *argv[] = { "./topdax", NULL };
	const struct application_ops quit_ops = {
		.startup = mock_startup,
		/* call quit on activation */
		.activate = application_quit,
		.shutdown = mock_shutdown,
	};
	struct application app = {
		.ops = &quit_ops,
	};
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(GLFW_TRUE));
	expect(mock_startup, when(obj, is_equal_to(&app)));
	expect(glfwWaitEvents);
	expect(mock_shutdown, when(obj, is_equal_to(&app)));
	expect(glfwTerminate);
	int exit_code = application_run(&app, 1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_SUCCESS));
}

static void mock_close_request(struct app_window *obj)
{
	mock(obj);
}

static const struct app_window_ops win_ops = {
	.close = mock_close_request,
};

Ensure(win_init_shows_window)
{
	struct app_window win = {
		.width = 960,
		.height = 540,
		.caption = "Topdax",
		.ops = &win_ops,
	};
	GLFWwindow *window = (GLFWwindow *) 1;
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(window),
	       when(width, is_equal_to(win.width)),
	       when(height, is_equal_to(win.height)),
	       when(title, is_equal_to_string(win.caption)));
	expect(glfwSetWindowUserPointer);
	expect(glfwSetWindowCloseCallback);
	int error = app_window_init(&win);
	assert_that(error, is_equal_to(0));
}

Ensure(win_init_returns_non_zero_on_fail)
{
	struct app_window win = {
		.width = 960,
		.height = 540,
		.caption = "Topdax",
		.ops = &win_ops,
	};
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(NULL),
	       when(width, is_equal_to(win.width)),
	       when(height, is_equal_to(win.height)),
	       when(title, is_equal_to_string(win.caption)));
	int error = app_window_init(&win);
	assert_that(error, is_not_equal_to(0));
}

Ensure(win_close_calls_callback)
{
	struct app_window win = {
		.width = 960,
		.height = 540,
		.caption = "Topdax",
		.ops = &win_ops,
	};
	GLFWwindow *window = (GLFWwindow *) 1;
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(window));
	expect(glfwSetWindowUserPointer);
	expect(glfwSetWindowCloseCallback);
	app_window_init(&win);
	expect(glfwGetWindowUserPointer, will_return(&win));
	expect(mock_close_request, when(obj, is_equal_to(&win)));
	app_window_close(&win);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("GLFW");
	TestSuite *app = create_named_test_suite("Application");
	add_test(app, app_calls_callbacks);
	add_test(app, app_exit_with_error_on_glfw_failure);
	add_test(app, app_accepts_help_argument_when_summary_provided);
	add_test(app, app_quit_ends_the_main_loop);
	add_suite(suite, app);

	TestSuite *win = create_named_test_suite("Window");
	add_test(win, win_init_shows_window);
	add_test(win, win_init_returns_non_zero_on_fail);
	add_test(win, win_close_calls_callback);
	add_suite(suite, win);
	return run_test_suite(suite, create_text_reporter());
}