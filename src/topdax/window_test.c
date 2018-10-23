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

GLFWAPI void glfwSetWindowUserPointer(GLFWwindow * window, void *pointer)
{
	mock(window, pointer);
}

GLFWAPI VkResult
glfwCreateWindowSurface(VkInstance instance, GLFWwindow * window,
			const VkAllocationCallbacks * allocator,
			VkSurfaceKHR * surface)
{
	return (VkResult) mock(instance, window, allocator, surface);
}

int vkrenderer_init(struct vkrenderer *rdr, VkInstance instance,
		    VkSurfaceKHR surface)
{
	return (int)mock(rdr, instance, surface);
}

void vkrenderer_terminate(const struct vkrenderer *rdr)
{
	mock(rdr);
}

VkResult vkrenderer_render(const struct vkrenderer *rdr)
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
	struct topdax_window win = { 0 };
	VkInstance vk = VK_NULL_HANDLE;
	expect(glfwWindowHint,
	       when(hint, is_equal_to(GLFW_CLIENT_API)),
	       when(value, is_equal_to(GLFW_NO_API)));
	expect(glfwCreateWindow,
	       will_return(window),
	       when(width, is_equal_to(960)),
	       when(height, is_equal_to(540)),
	       when(title, is_equal_to_string("Topdax")));
	expect(glfwSetWindowUserPointer);
	expect(glfwCreateWindowSurface, will_return(VK_SUCCESS));
	expect(vkrenderer_init, when(rdr, is_equal_to(&win.renderer)));
	expect(vkrenderer_render, when(rdr, is_equal_to(&win.renderer)));
	int error = topdax_window_init(&win, vk);
	assert_that(error, is_equal_to(0));
}

Ensure(topdax_window_init_returns_non_zero_on_fail)
{
	struct topdax_window win = { 0 };
	VkInstance vk = VK_NULL_HANDLE;
	expect(glfwWindowHint,
	       when(hint, is_equal_to(GLFW_CLIENT_API)),
	       when(value, is_equal_to(GLFW_NO_API)));
	expect(glfwCreateWindow, will_return(NULL));
	int error = topdax_window_init(&win, vk);
	assert_that(error, is_not_equal_to(0));
}

Ensure(topdax_window_init_returns_non_zero_on_surface_fail)
{
	GLFWwindow *window = (GLFWwindow *) 1;
	struct topdax_window win = { 0 };
	VkInstance vk = VK_NULL_HANDLE;
	expect(glfwWindowHint,
	       when(hint, is_equal_to(GLFW_CLIENT_API)),
	       when(value, is_equal_to(GLFW_NO_API)));
	expect(glfwCreateWindow, will_return(window));
	expect(glfwSetWindowUserPointer);
	expect(glfwCreateWindowSurface,
	       will_return(VK_ERROR_INITIALIZATION_FAILED));
	never_expect(vkrenderer_init);
	int error = topdax_window_init(&win, vk);
	assert_that(error, is_not_equal_to(0));
}

Ensure(topdax_window_destroy_destroys_window)
{
	struct topdax_window win = { 0 };
	expect(vkrenderer_terminate, when(rdr, is_equal_to(&win.renderer)));
	expect(vkDestroySurfaceKHR, when(surface, is_equal_to(win.surface)));
	topdax_window_destroy(&win);
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
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
