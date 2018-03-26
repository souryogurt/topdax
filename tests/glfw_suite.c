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
	expect(glfwInit, will_return(GLFW_TRUE));
	expect(mock_startup, when(obj, is_equal_to(&app)));
	expect(glfwPollEvents);
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
	add_test(app, app_exit_with_error_on_unknown_flag);
	add_test(app, app_quit_ends_the_main_loop);
	add_suite(suite, app);

	TestSuite *win = create_named_test_suite("Window");
	add_test(win, win_init_shows_window);
	add_test(win, win_init_returns_non_zero_on_fail);
	add_test(win, win_close_calls_callback);
	add_suite(suite, win);
	return run_test_suite(suite, create_text_reporter());
}
