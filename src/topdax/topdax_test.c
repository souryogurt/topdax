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

#include <application/utils.h>
#include <GLFW/glfw3.h>
#include "topdax.h"

struct topdax_window;

GLFWAPI int glfwInit(void)
{
	return (int)mock();
}

GLFWAPI void glfwTerminate(void)
{
	mock();
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo *
						pCreateInfo,
						const VkAllocationCallbacks *
						pAllocator,
						VkInstance * pInstance)
{
	return (VkResult) mock(pCreateInfo, pAllocator, pInstance);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance,
					     const VkAllocationCallbacks *
					     pAllocator)
{
	mock(instance, pAllocator);
}

GLFWAPI const char **glfwGetRequiredInstanceExtensions(uint32_t * count)
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

int topdax_window_init(struct topdax_window *win, struct topdax *app)
{
	return (int)mock(win, app);
}

void topdax_window_destroy(struct topdax_window *win)
{
	mock(win);
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

GLFWAPI void glfwWaitEvents(void)
{
	mock();
}

Ensure(main_returns_zero_on_success)
{
	const char *wsi_exts[] = { "VK_KHR_surface" };
	const uint32_t wsi_size = ARRAY_SIZE(wsi_exts);
	char *argv[] = { "./topdax", NULL };
	expect(__wrap_argp_parse);
	expect(glfwInit, will_return(1));
	expect(glfwGetRequiredInstanceExtensions,
	       will_return(wsi_exts),
	       will_set_contents_of_parameter(count, &wsi_size,
					      sizeof(uint32_t)));
	expect(vkCreateInstance);
#ifndef NDEBUG
	expect(setup_debug_logger);
#endif
	expect(topdax_window_init);
	expect(glfwWaitEvents);
	expect(topdax_window_destroy);
#ifndef NDEBUG
	expect(destroy_debug_logger);
#endif
	expect(vkDestroyInstance);
	expect(glfwTerminate);
	application_quit();
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
	expect(glfwGetRequiredInstanceExtensions,
	       will_return(wsi_exts),
	       will_set_contents_of_parameter(count, &wsi_size,
					      sizeof(uint32_t)));
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
	return run_test_suite(suite, create_text_reporter());
}
