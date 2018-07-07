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
#include "topdax/topdax.h"
#include <GLFW/window.h>
#include <GLFW/runloop.h>
#include "topdax/vkrenderer.h"

int glfw_window_init(struct glfw_window *win, int width, int height,
		     const char *caption, struct window_handler *wh)
{
	return (int)mock(win, width, height, caption, wh);
}

void runloop_quit(struct runloop *loop)
{
	mock(loop);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo *
						pCreateInfo,
						const VkAllocationCallbacks *
						pAllocator,
						VkInstance * pInstance)
{
	return mock(pCreateInfo, pAllocator, pInstance);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance,
					     const VkAllocationCallbacks *
					     pAllocator)
{
	mock(instance, pAllocator);
}

int vkrenderer_init(struct vkrenderer *rdr)
{
	return mock(rdr);
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	mock(rdr);
}

Ensure(topdax_activate_creates_window)
{
	expect(glfw_window_init,
	       when(width, is_equal_to(960)),
	       when(height, is_equal_to(540)),
	       when(caption, is_equal_to_string("Topdax")));
	expect(vkrenderer_init);
	application_activate();
}

Ensure(topdax_close_main_window_ends_application)
{
	struct runloop_ops loop_ops = {
		.quit = &runloop_quit,
	};
	struct runloop loop = {
		.ops = &loop_ops,
	};
	expect(vkCreateInstance);
	expect(runloop_quit, when(loop, is_equal_to(&loop)));
	application_startup(&loop);
	topdax_close_window(NULL, NULL);
}

Ensure(topdax_startup_initializes_components)
{
	struct runloop loop;
	expect(vkCreateInstance);
	application_startup(&loop);
}

Ensure(topdax_shutdown_terminates_components)
{
	expect(vkrenderer_terminate);
	expect(vkDestroyInstance);
	application_shutdown();
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *tpx = create_named_test_suite("Topdax");
	add_test(tpx, topdax_activate_creates_window);
	add_test(tpx, topdax_close_main_window_ends_application);
	add_test(tpx, topdax_startup_initializes_components);
	add_test(tpx, topdax_shutdown_terminates_components);
	return run_test_suite(tpx, create_text_reporter());
}
