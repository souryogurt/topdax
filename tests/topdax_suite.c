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

int application_run(struct application *app, int argc, char **argv)
{
	return (int)mock(app, argc, argv);
}

int app_window_init(struct app_window *win)
{
	return (int)mock(win);
}

void application_quit(struct application *app)
{
	mock(app);
}

int vkrenderer_init(struct vkrenderer *rdr)
{
	return mock(rdr);
}

int vkrenderer_terminate(struct vkrenderer *rdr)
{
	return mock(rdr);
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
	struct topdax tpx;
	expect(app_window_init, when(win, is_equal_to(&tpx.win)));
	topdax_activate(&tpx.app);
}

Ensure(topdax_close_main_window_ends_application)
{
	struct topdax tpx;
	expect(application_quit, when(app, is_equal_to(&tpx.app)));
	topdax_close_window(&tpx.win);
}

Ensure(topdax_startup_initializes_components)
{
	struct topdax tpx = { 0 };
	expect(vkrenderer_init, when(rdr, is_equal_to(tpx.rdr)));
	topdax_startup(&tpx.app);
}

Ensure(topdax_shutdown_terminates_components)
{
	struct topdax tpx = { 0 };
	expect(vkrenderer_terminate, when(rdr, is_equal_to(tpx.rdr)));
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
