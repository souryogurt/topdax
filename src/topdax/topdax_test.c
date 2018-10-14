/**
 * @file
 * Test suite for topdax
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>
#include <unistd.h>
#include "topdax.h"
#include <renderer/vkrenderer.h>
#include <application/utils.h>

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

Ensure(topdax_startup_initializes_components)
{
	const char *wsi_exts[] = { "VK_KHR_surface" };
	const uint32_t wsi_size = ARRAY_SIZE(wsi_exts);
	struct runloop loop;
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
	application_startup(&loop);
}

Ensure(topdax_startup_fails_on_glfw_fail)
{
	struct runloop loop;
	expect(glfwInit, will_return(0));
	never_expect(vkCreateInstance);
	never_expect(topdax_window_init);
	int status = application_startup(&loop);
	assert_that(status, is_not_equal_to(0));
}

Ensure(topdax_startup_fails_on_out_of_memory_for_extensions)
{
	struct runloop loop;
	const char *wsi_exts[101];
	const uint32_t wsi_count = ARRAY_SIZE(wsi_exts);
	expect(glfwInit, will_return(1));
	expect(glfwGetRequiredInstanceExtensions,
	       will_return(wsi_exts),
	       will_set_contents_of_parameter(count, &wsi_count,
					      sizeof(uint32_t)));
	never_expect(vkCreateInstance);
	never_expect(topdax_window_init);
	int status = application_startup(&loop);
	assert_that(status, is_not_equal_to(0));
}

Ensure(topdax_shutdown_terminates_components)
{
	expect(topdax_window_destroy);
#ifndef NDEBUG
	expect(destroy_debug_logger);
#endif
	expect(vkDestroyInstance);
	expect(glfwTerminate);
	application_shutdown();
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("Topdax");
	add_test(suite, topdax_startup_initializes_components);
	add_test(suite, topdax_startup_fails_on_glfw_fail);
	add_test(suite, topdax_startup_fails_on_out_of_memory_for_extensions);
	add_test(suite, topdax_shutdown_terminates_components);
	return run_test_suite(suite, create_text_reporter());
}
