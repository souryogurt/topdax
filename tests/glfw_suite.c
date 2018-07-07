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
#include "topdax/application.h"
#include <GLFW/window.h>
#include <GLFW/runloop.h>
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

static struct runloop *g_loop;

static void mock_startup(struct runloop *obj)
{
	mock(obj);
	g_loop = obj;
}

static void mock_activate()
{
	mock();
	g_loop->ops->quit(g_loop);
}

static void mock_shutdown()
{
	mock();
}

static const struct application_ops test_ops = {
	.startup = mock_startup,
	.activate = mock_activate,
	.shutdown = mock_shutdown,
};

Ensure(app_calls_callbacks)
{
	struct glfw_runloop loop;
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(GLFW_TRUE));
	expect(mock_startup, when(obj, is_equal_to(&loop)));
	expect(mock_activate);
	expect(glfwWaitEvents);
	expect(mock_shutdown);
	expect(glfwTerminate);
	int exit_code = glfw_runloop_run(&loop, &test_ops, NULL, 1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_SUCCESS));
}

Ensure(app_exit_with_error_on_glfw_failure)
{
	struct glfw_runloop loop;
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(GLFW_FALSE));
	never_expect(mock_startup);
	never_expect(mock_activate);
	never_expect(mock_shutdown);
	int exit_code = glfw_runloop_run(&loop, &test_ops, NULL, 1, &argv[0]);
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
	struct glfw_runloop loop;
	expect(__wrap_argp_parse, will_return(EXIT_FAILURE),
	       when(argp_doc, is_equal_to_string(info.summary)),
	       when(argp_version, is_equal_to_string(info.version)),
	       when(argp_bug_address, is_equal_to_string(info.bug_address)));
	int exit_code = glfw_runloop_run(&loop, &test_ops, &info, 1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

static void mock_close_request(struct window_handler *obj, struct window *win)
{
	mock(obj, win);
}

static const struct window_handler_ops win_ops = {
	.close = mock_close_request,
};

Ensure(win_init_shows_window)
{
	struct window_handler wh = {
		.ops = &win_ops,
	};
	struct glfw_window win;
	GLFWwindow *window = (GLFWwindow *) 1;
	expect(glfwWindowHint);
	expect(glfwCreateWindow,
	       will_return(window),
	       when(width, is_equal_to(960)),
	       when(height, is_equal_to(540)),
	       when(title, is_equal_to_string("Topdax")));
	expect(glfwSetWindowUserPointer);
	expect(glfwSetWindowCloseCallback);
	int error = glfw_window_init(&win, 960, 540, "Topdax", &wh);
	assert_that(error, is_equal_to(0));
}

Ensure(win_init_returns_non_zero_on_fail)
{
	struct window_handler wh = {
		.ops = &win_ops,
	};
	struct glfw_window win;
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(NULL));
	int error = glfw_window_init(&win, 960, 540, "Topdax", &wh);
	assert_that(error, is_not_equal_to(0));
}

Ensure(win_close_calls_callback)
{
	struct window_handler wh = {
		.ops = &win_ops,
	};
	struct glfw_window win;
	GLFWwindow *window = (GLFWwindow *) 1;
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(window));
	expect(glfwSetWindowUserPointer);
	expect(glfwSetWindowCloseCallback);
	glfw_window_init(&win, 960, 540, "Topdax", &wh);
	expect(glfwGetWindowUserPointer, will_return(&win));
	expect(mock_close_request,
	       when(obj, is_equal_to(&wh)), when(win, is_equal_to(&win)));
	win.win.ops->close(&win.win);
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
	add_suite(suite, app);

	TestSuite *win = create_named_test_suite("Window");
	add_test(win, win_init_shows_window);
	add_test(win, win_init_returns_non_zero_on_fail);
	add_test(win, win_close_calls_callback);
	add_suite(suite, win);
	return run_test_suite(suite, create_text_reporter());
}
