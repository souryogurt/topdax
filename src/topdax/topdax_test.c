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
#include <GLFW/window.h>
#include <GLFW/runloop.h>
#include <renderer/vkrenderer.h>

GLFWAPI int glfwInit(void)
{
	return (int)mock();
}

GLFWAPI void glfwTerminate(void)
{
	mock();
}

int glfw_window_init(struct glfw_window *win, int width, int height,
		     const char *caption, struct window_handler *winh)
{
	return (int)mock(win, width, height, caption, winh);
}

static void runloop_quit(struct runloop *loop)
{
	mock(loop);
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

int vkrenderer_init(struct vkrenderer *rdr)
{
	return (int)mock(rdr);
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	mock(rdr);
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
	expect(glfw_window_init,
	       when(width, is_equal_to(960)),
	       when(height, is_equal_to(540)),
	       when(caption, is_equal_to_string("Topdax")));
	expect(vkrenderer_init);
	application_startup(&loop);
}

Ensure(topdax_startup_fails_on_glfw_fail)
{
	struct runloop loop;
	expect(glfwInit, will_return(0));
	never_expect(vkCreateInstance);
	never_expect(glfw_window_init);
	never_expect(vkrenderer_init);
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
	never_expect(glfw_window_init);
	never_expect(vkrenderer_init);
	int status = application_startup(&loop);
	assert_that(status, is_not_equal_to(0));
}

Ensure(topdax_shutdown_terminates_components)
{
	expect(vkrenderer_terminate);
#ifndef NDEBUG
	expect(destroy_debug_logger);
#endif
	expect(vkDestroyInstance);
	expect(glfwTerminate);
	application_shutdown();
}

Ensure(topdax_close_main_window_ends_application)
{
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
	expect(runloop_quit, when(loop, is_equal_to(&loop)));
	topdax_close_window(&app.window.handler, &app.window.window.win);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("Topdax");
	TestSuite *tpx = create_named_test_suite("Application");
	add_test(tpx, topdax_startup_initializes_components);
	add_test(tpx, topdax_startup_fails_on_glfw_fail);
	add_test(tpx, topdax_startup_fails_on_out_of_memory_for_extensions);
	add_test(tpx, topdax_shutdown_terminates_components);
	add_suite(suite, tpx);

	TestSuite *win = create_named_test_suite("Window");
	add_test(win, topdax_close_main_window_ends_application);
	add_suite(suite, win);

	return run_test_suite(suite, create_text_reporter());
}
