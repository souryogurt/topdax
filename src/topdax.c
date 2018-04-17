/**
 * @file
 * Topdax application
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "topdax/topdax.h"

void topdax_activate(struct application *obj)
{
	struct topdax *app = container_of(obj, struct topdax, app);
	app_window_init(&app->win);
}

void topdax_close_window(struct app_window *obj)
{
	struct topdax *app = container_of(obj, struct topdax, win);
	application_quit(&app->app);
}

/** Implementation of main Topdax window */
static const struct app_window_ops topdax_window_ops = {
	.close = topdax_close_window,
};

/** Implementation of Topdax application */
static const struct application_ops topdax_ops = {
	.activate = topdax_activate,
};

static const struct application_info topdax_info = {
	.version = PACKAGE_STRING,
	.bug_address = PACKAGE_BUGREPORT,
	.summary = "The program that renders triangle using Vulkan API",
};

/** Topdax application instance */
static struct topdax app = {
	.app = {
		.info = &topdax_info,
		.ops = &topdax_ops,
		},
	.win = {
		.width = 960,
		.height = 540,
		.caption = "Topdax",
		.ops = &topdax_window_ops,
		},
};

int topdax_run(int argc, char **argv)
{
	return application_run(&app.app, argc, argv);
}
