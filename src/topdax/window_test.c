/**
 * @file
 * Test suite for topdax window
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "topdax.h"
#include "window.h"

struct vkrenderer;

GLFWAPI void glfwWindowHint(int hint, int value)
{
	mock(hint, value);
}

GLFWAPI GLFWwindow *glfwCreateWindow(int width, int height, const char *title,
				     GLFWmonitor * monitor, GLFWwindow * share)
{
	return (GLFWwindow *) mock(width, height, title, monitor, share);
}

static GLFWwindowclosefun glfw_close_request;

GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow * window,
						      GLFWwindowclosefun cbfun)
{
	/* Store callback to be able to simulate close request in tests */
	glfw_close_request = cbfun;
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

GLFWAPI VkResult
glfwCreateWindowSurface(VkInstance instance, GLFWwindow * window,
			const VkAllocationCallbacks * allocator,
			VkSurfaceKHR * surface)
{
	return (VkResult) mock(instance, window, allocator, surface);
}

static void runloop_quit(struct runloop *loop)
{
	mock(loop);
}

int vkrenderer_init(struct vkrenderer *rdr, VkInstance instance,
		    VkSurfaceKHR surface)
{
	return (int)mock(rdr, instance, surface);
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	mock(rdr);
}

VkResult vkrenderer_render(struct vkrenderer *rdr)
{
	return (VkResult) mock(rdr);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface,
		    const VkAllocationCallbacks * pAllocator)
{
	mock(instance, surface, pAllocator);
}

Ensure(topdax_window_init_creates_window)
{
	GLFWwindow *window = (GLFWwindow *) 1;
	struct topdax_window win;
	struct topdax app;
	expect(glfwWindowHint);
	expect(glfwCreateWindow,
	       will_return(window),
	       when(width, is_equal_to(960)),
	       when(height, is_equal_to(540)),
	       when(title, is_equal_to_string("Topdax")));
	expect(glfwSetWindowUserPointer);
	expect(glfwSetWindowCloseCallback);
	expect(glfwCreateWindowSurface, will_return(VK_SUCCESS));
	expect(vkrenderer_init, when(rdr, is_equal_to(&win.renderer)));
	expect(vkrenderer_render, when(rdr, is_equal_to(&win.renderer)));
	int error = topdax_window_init(&win, &app);
	assert_that(error, is_equal_to(0));
}

Ensure(topdax_window_init_returns_non_zero_on_fail)
{
	struct topdax_window win;
	struct topdax app;
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(NULL));
	never_expect(glfwSetWindowUserPointer);
	never_expect(glfwSetWindowCloseCallback);
	never_expect(vkrenderer_init);
	int error = topdax_window_init(&win, &app);
	assert_that(error, is_not_equal_to(0));
}

Ensure(topdax_window_init_returns_non_zero_on_surface_fail)
{
	GLFWwindow *window = (GLFWwindow *) 1;
	struct topdax_window win;
	struct topdax app;
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(window));
	expect(glfwSetWindowUserPointer);
	expect(glfwSetWindowCloseCallback);
	expect(glfwCreateWindowSurface,
	       will_return(VK_ERROR_INITIALIZATION_FAILED));
	never_expect(vkrenderer_init);
	int error = topdax_window_init(&win, &app);
	assert_that(error, is_not_equal_to(0));
}

Ensure(topdax_window_destroy_destroys_window)
{
	struct topdax app;
	struct topdax_window win = {
		.app = &app,
	};
	expect(vkrenderer_terminate, when(rdr, is_equal_to(&win.renderer)));
	expect(vkDestroySurfaceKHR, when(surface, is_equal_to(win.surface)));
	topdax_window_destroy(&win);
}

Ensure(topdax_window_ends_application_on_close_request)
{
	GLFWwindow *window = (GLFWwindow *) 1;
	struct runloop_ops loop_ops = {
		.quit = &runloop_quit,
	};
	struct runloop loop = {
		.ops = &loop_ops,
	};
	struct topdax app = {
		.mainloop = &loop,
		.window = {
			   .app = &app,
			   },
	};
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(window));
	expect(glfwSetWindowUserPointer);
	expect(glfwSetWindowCloseCallback);
	expect(glfwCreateWindowSurface, will_return(VK_SUCCESS));
	expect(vkrenderer_init);
	expect(vkrenderer_render, will_return(VK_SUCCESS));
	int error = topdax_window_init(&app.window, &app);
	assert_that(error, is_equal_to(0));

	expect(glfwGetWindowUserPointer,
	       will_return(&app.window), when(window, is_equal_to(window)));
	expect(runloop_quit, when(loop, is_equal_to(&loop)));
	glfw_close_request(app.window.id);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("Topdax Window");
	add_test(suite, topdax_window_init_creates_window);
	add_test(suite, topdax_window_init_returns_non_zero_on_fail);
	add_test(suite, topdax_window_init_returns_non_zero_on_surface_fail);
	add_test(suite, topdax_window_destroy_destroys_window);
	add_test(suite, topdax_window_ends_application_on_close_request);
	return run_test_suite(suite, create_text_reporter());
}
