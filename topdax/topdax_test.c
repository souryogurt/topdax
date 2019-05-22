/**
 * @file
 * Test suite for topdax
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <argp.h>
#include <stdint.h>
#include <stdlib.h>

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include "topdax.h"

struct vkrenderer;

GLFWAPI int glfwInit(void)
{
	return (int)mock();
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
				     GLFWmonitor *monitor, GLFWwindow *share)
{
	return (GLFWwindow *)mock(width, height, title, monitor, share);
}

GLFWAPI void glfwDestroyWindow(GLFWwindow *window)
{
	mock(window);
}

GLFWAPI VkResult glfwCreateWindowSurface(VkInstance instance,
					 GLFWwindow *window,
					 const VkAllocationCallbacks *allocator,
					 VkSurfaceKHR *surface)
{
	return (VkResult)mock(instance, window, allocator, surface);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface,
		    const VkAllocationCallbacks *pAllocator)
{
	mock(instance, surface, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateInstance(const VkInstanceCreateInfo *pCreateInfo,
		 const VkAllocationCallbacks *pAllocator, VkInstance *pInstance)
{
	return (VkResult)mock(pCreateInfo, pAllocator, pInstance);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator)
{
	mock(instance, pAllocator);
}

GLFWAPI const char **glfwGetRequiredInstanceExtensions(uint32_t *count)
{
	return (const char **)mock(count);
}

#ifndef NDEBUG
void setup_debug_logger(VkInstance instance)
{
	mock(instance);
}

void destroy_debug_logger(VkInstance instance)
{
	mock(instance);
}
#endif

error_t __wrap_argp_parse(const struct argp *__restrict __argp, int __argc,
			  char **__restrict __argv, unsigned __flags,
			  int *__restrict __arg_index, void *__restrict __input)
{
	const char *argp_doc = __argp->doc;
	const char *argp_version = argp_program_version;
	const char *argp_bug_address = argp_program_bug_address;
	return (error_t)mock(__argp, __argc, __argv, __flags, __arg_index,
			     __input, argp_doc, argp_version, argp_bug_address);
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

GLFWAPI void glfwPollEvents(void)
{
	mock();
}

GLFWAPI int glfwWindowShouldClose(GLFWwindow *window)
{
	return (int)mock(window);
}

int vkrenderer_render(struct vkrenderer *rdr)
{
	return (int)mock(rdr);
}

Ensure(main_returns_zero_on_success)
{
	const char *wsi_exts[] = { "VK_KHR_surface" };
	const uint32_t wsi_size = ARRAY_SIZE(wsi_exts);
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(1));
	expect(glfwGetRequiredInstanceExtensions, will_return(wsi_exts),
	       will_set_contents_of_parameter(count, &wsi_size,
					      sizeof(wsi_size)));
	expect(vkCreateInstance);
#ifndef NDEBUG
	expect(setup_debug_logger);
#endif
	expect(glfwWindowHint, when(hint, is_equal_to(GLFW_CLIENT_API)),
	       when(value, is_equal_to(GLFW_NO_API)));
	expect(glfwCreateWindow, will_return((GLFWwindow *)1),
	       when(width, is_equal_to(960)), when(height, is_equal_to(540)),
	       when(title, is_equal_to_string("Topdax")));
	expect(glfwCreateWindowSurface, will_return(VK_SUCCESS));
	expect(vkrenderer_init);
	expect(glfwWindowShouldClose, will_return(0));
	expect(glfwPollEvents);
	expect(vkrenderer_render);
	expect(glfwWindowShouldClose, will_return(1));
	expect(vkrenderer_terminate);
	expect(vkDestroySurfaceKHR);
	expect(glfwDestroyWindow, when(window, is_equal_to((GLFWwindow *)1)));
#ifndef NDEBUG
	expect(destroy_debug_logger);
#endif
	expect(vkDestroyInstance);
	expect(glfwTerminate);
	int exit_code = application_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_SUCCESS));
}

Ensure(main_returns_non_zero_when_help_requested)
{
	char *argv[] = { "./topdax", "--help", NULL };
	expect(__wrap_argp_parse, will_return(EXIT_FAILURE));
	int exit_code = application_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(main_returns_non_zero_on_glfw_fail)
{
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(0));
	int exit_code = application_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(main_returns_non_zero_on_vulkan_instance_init_fail)
{
	const char *wsi_exts[101];
	const uint32_t wsi_size = ARRAY_SIZE(wsi_exts);
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse, will_return(EXIT_SUCCESS));
	expect(glfwInit, will_return(1));
	expect(glfwGetRequiredInstanceExtensions, will_return(wsi_exts),
	       will_set_contents_of_parameter(count, &wsi_size,
					      sizeof(wsi_size)));
	expect(glfwTerminate);
	int exit_code = application_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(main_returns_non_zero_on_window_init_fail)
{
	const char *wsi_exts[] = { "VK_KHR_surface" };
	const uint32_t wsi_size = ARRAY_SIZE(wsi_exts);
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(1));
	expect(glfwGetRequiredInstanceExtensions, will_return(wsi_exts),
	       will_set_contents_of_parameter(count, &wsi_size,
					      sizeof(wsi_size)));
	expect(vkCreateInstance);
#ifndef NDEBUG
	expect(setup_debug_logger);
#endif
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return(NULL));
	expect(vkDestroyInstance);
	expect(glfwTerminate);
	int exit_code = application_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(main_returns_non_zero_on_window_surface_init_fail)
{
	const char *wsi_exts[] = { "VK_KHR_surface" };
	const uint32_t wsi_size = ARRAY_SIZE(wsi_exts);
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(1));
	expect(glfwGetRequiredInstanceExtensions, will_return(wsi_exts),
	       will_set_contents_of_parameter(count, &wsi_size,
					      sizeof(wsi_size)));
	expect(vkCreateInstance);
#ifndef NDEBUG
	expect(setup_debug_logger);
#endif
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return((GLFWwindow *)1));
	expect(glfwCreateWindowSurface, will_return(VK_ERROR_DEVICE_LOST));
	expect(glfwDestroyWindow);
#ifndef NDEBUG
	expect(destroy_debug_logger);
#endif
	expect(vkDestroyInstance);
	expect(glfwTerminate);
	int exit_code = application_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

Ensure(main_returns_non_zero_on_renderer_init_fail)
{
	const char *wsi_exts[] = { "VK_KHR_surface" };
	const uint32_t wsi_size = ARRAY_SIZE(wsi_exts);
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(1));
	expect(glfwGetRequiredInstanceExtensions, will_return(wsi_exts),
	       will_set_contents_of_parameter(count, &wsi_size,
					      sizeof(wsi_size)));
	expect(vkCreateInstance);
#ifndef NDEBUG
	expect(setup_debug_logger);
#endif
	expect(glfwWindowHint);
	expect(glfwCreateWindow, will_return((GLFWwindow *)1));
	expect(glfwCreateWindowSurface, will_return(VK_SUCCESS));
	expect(vkrenderer_init, will_return(1));
	expect(vkDestroySurfaceKHR);
	expect(glfwDestroyWindow);
#ifndef NDEBUG
	expect(destroy_debug_logger);
#endif
	expect(vkDestroyInstance);
	expect(glfwTerminate);
	int exit_code = application_main(1, &argv[0]);
	assert_that(exit_code, is_equal_to(EXIT_FAILURE));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("Topdax");
	add_test(suite, main_returns_zero_on_success);
	add_test(suite, main_returns_non_zero_on_glfw_fail);
	add_test(suite, main_returns_non_zero_on_vulkan_instance_init_fail);
	add_test(suite, main_returns_non_zero_when_help_requested);
	add_test(suite, main_returns_non_zero_on_window_init_fail);
	add_test(suite, main_returns_non_zero_on_window_surface_init_fail);
	add_test(suite, main_returns_non_zero_on_renderer_init_fail);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
