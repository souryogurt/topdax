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
#include <threads.h>

struct topdax_window;
struct vkrenderer;

GLFWAPI int glfwInit(void)
{
	return (int)mock();
}

GLFWAPI void glfwTerminate(void)
{
	mock();
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

int topdax_window_init(struct topdax_window *win, VkInstance instance)
{
	return (int)mock(win, instance);
}

void topdax_window_destroy(struct topdax_window *win)
{
	mock(win);
}

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

GLFWAPI void glfwWaitEvents(void)
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

int thrd_create(thrd_t *__thr, thrd_start_t __func, void *__arg)
{
	return (int)mock(__thr, __func, __arg);
}

int thrd_join(thrd_t __thr, int *__res)
{
	return (int)mock(__thr, __res);
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
	expect(topdax_window_init);
	expect(vkrenderer_init);
	expect(thrd_create);
	expect(glfwWindowShouldClose, will_return(0));
	expect(glfwWaitEvents);
	expect(glfwWindowShouldClose, will_return(1));
	expect(thrd_join);
	expect(vkrenderer_terminate);
	expect(topdax_window_destroy);
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
	expect(topdax_window_init, will_return(1));
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
	expect(topdax_window_init);
	expect(vkrenderer_init, will_return(1));
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
	add_test(suite, main_returns_non_zero_on_renderer_init_fail);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
