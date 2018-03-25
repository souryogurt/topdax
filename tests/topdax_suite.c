/**
 * @file
 * Test suite for topdax
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>
#include <unistd.h>
#include "topdax/topdax.h"
#include "topdax/application.h"
#include <GLFW/glfw3.h>

GLFWAPI int glfwInit(void)
{
	return mock();
}

GLFWAPI GLFWwindow *glfwCreateWindow(int width, int height, const char *title,
				     GLFWmonitor * monitor, GLFWwindow * share)
{
	return (GLFWwindow *) mock(width, height, title, monitor, share);
}

GLFWAPI int glfwWindowShouldClose(GLFWwindow * window)
{
	return mock(window);
}

GLFWAPI void glfwPollEvents(void)
{
	mock();
}

GLFWAPI void glfwTerminate(void)
{
	mock();
}

GLFWAPI void glfwDestroyWindow(GLFWwindow * window)
{
	mock(window);
}

GLFWAPI void glfwWindowHint(int hint, int value)
{
	mock(hint, value);
}

Ensure(topdax_exit_with_error_on_glfw_failure)
{
	char *argv[] = { "./topdax", NULL };
	expect(glfwInit, will_return(GLFW_FALSE));
	int exit_code = topdax_run(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(topdax_exit_with_error_on_window_failure)
{
	char *argv[] = { "./topdax", NULL };
	expect(glfwInit, will_return(GLFW_TRUE));
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(NULL));
	expect(glfwTerminate);
	int exit_code = topdax_run(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(topdax_shows_window)
{
	char *argv[] = { "./topdax", NULL };
	GLFWwindow *window = (GLFWwindow *) 1;
	expect(glfwInit, will_return(GLFW_TRUE));
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(window),
	       when(width, is_equal_to(960)),
	       when(height, is_equal_to(540)),
	       when(title, is_equal_to_string("Topdax")));
	expect(glfwWindowShouldClose, will_return(GLFW_FALSE),
	       when(window, is_equal_to(window)));
	expect(glfwPollEvents);
	expect(glfwWindowShouldClose, will_return(GLFW_TRUE),
	       when(window, is_equal_to(window)));
	expect(glfwDestroyWindow, when(window, is_equal_to(window)));
	expect(glfwTerminate);
	int exit_code = topdax_run(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_SUCCESS));
}

Ensure(topdax_exit_with_error_on_unknown_flag)
{
	char *argv[] = { "./topdax", "--unsupported", NULL };
	/* Suppress stderr */
	close(2);
	int exit_code = topdax_run(2, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("Topdax");
	TestSuite *tpx = create_named_test_suite("Topdax");
	add_test(tpx, topdax_exit_with_error_on_glfw_failure);
	add_test(tpx, topdax_exit_with_error_on_window_failure);
	add_test(tpx, topdax_exit_with_error_on_unknown_flag);
	add_test(tpx, topdax_shows_window);
	add_suite(suite, tpx);
	return run_test_suite(suite, create_text_reporter());
}
