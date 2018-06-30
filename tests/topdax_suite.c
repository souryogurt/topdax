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
#include "topdax/vkrenderer.h"

int application_run(struct application *app, int argc, char **argv)
{
	return (int)mock(app, argc, argv);
}

int window_init(struct window *win)
{
	return (int)mock(win);
}

void application_quit(struct application *app)
{
	mock(app);
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

Ensure(topdax_run_calls_application_run)
{
	char *argv[] = { "./topdax", NULL };
	expect(application_run, will_return(0), when(argc, is_equal_to(1)),
	       when(argv, is_equal_to(&argv[0])));
	int exit_code = topdax_run(1, &argv[0]);
	assert_that(exit_code, is_equal_to(0));
}

Ensure(topdax_activate_creates_window)
{
	struct topdax_window main_window;
	struct topdax tpx= {
		.main_window = &main_window,
	};
	expect(window_init, when(win, is_equal_to(&tpx.main_window->win)));
	expect(vkrenderer_init, when(rdr, is_equal_to(tpx.rdr)));
	topdax_activate(&tpx.app);
}

Ensure(topdax_close_main_window_ends_application)
{
	struct topdax_window main_window;
	struct topdax tpx= {
		.main_window = &main_window,
	};
	main_window.app = &tpx.app;
	expect(application_quit, when(app, is_equal_to(&tpx.app)));
	topdax_close_window(&tpx.main_window->win);
}

Ensure(topdax_startup_initializes_components)
{
	struct vkrenderer renderer;
	struct topdax tpx = {.rdr = &renderer };
	expect(vkCreateInstance, when(pInstance, is_equal_to(&tpx.vk)));
	topdax_startup(&tpx.app);
}

Ensure(topdax_shutdown_terminates_components)
{
	struct vkrenderer renderer;
	struct topdax tpx = {.rdr = &renderer };
	expect(vkrenderer_terminate, when(rdr, is_equal_to(tpx.rdr)));
	expect(vkDestroyInstance, when(instance, is_equal_to(tpx.vk)));
	topdax_shutdown(&tpx.app);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *tpx = create_named_test_suite("Topdax");
	add_test(tpx, topdax_run_calls_application_run);
	add_test(tpx, topdax_activate_creates_window);
	add_test(tpx, topdax_close_main_window_ends_application);
	add_test(tpx, topdax_startup_initializes_components);
	add_test(tpx, topdax_shutdown_terminates_components);
	return run_test_suite(tpx, create_text_reporter());
}
