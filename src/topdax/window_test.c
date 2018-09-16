/**
 * @file
 * Test suite for topdax window
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <application/runloop.h>
#include "topdax.h"
#include "window.h"

static void runloop_quit(struct runloop *loop)
{
	mock(loop);
}

int vkrenderer_init(struct vkrenderer *rdr)
{
	return (int)mock(rdr);
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	mock(rdr);
}

int glfw_window_init(struct glfw_window *win, int width, int height,
		     const char *caption, struct window_handler *winh)
{
	return (int)mock(win, width, height, caption, winh);
}

Ensure(topdax_window_init_creates_window)
{
	struct topdax_window win;
	struct topdax app;
	expect(glfw_window_init,
	       when(win, is_equal_to(&win.window)),
	       when(width, is_equal_to(960)),
	       when(height, is_equal_to(540)),
	       when(caption, is_equal_to_string("Topdax")));
	expect(vkrenderer_init, when(rdr, is_equal_to(&win.renderer)));
	topdax_window_init(&win, &app);
}

Ensure(topdax_window_destroy_destroys_window)
{
	struct topdax_window win;
	expect(vkrenderer_terminate, when(rdr, is_equal_to(&win.renderer)));
	topdax_window_destroy(&win);
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
	TestSuite *suite = create_named_test_suite("Topdax Window");
	add_test(suite, topdax_window_init_creates_window);
	add_test(suite, topdax_window_destroy_destroys_window);
	add_test(suite, topdax_close_main_window_ends_application);
	return run_test_suite(suite, create_text_reporter());
}
