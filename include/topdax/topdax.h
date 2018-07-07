#ifndef TOPDAX_H
#define TOPDAX_H
#include "topdax/application.h"
#include <vulkan/vulkan.h>

struct vkrenderer;

/** Topdax Application */
struct topdax {
	/** Vulkan instance */
	VkInstance vk;
	/** Pointer to renderer */
	struct vkrenderer *rdr;
	/** Implements base application */
	struct application app;
	/** Implements window handler */
	struct window_handler handler;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Starts up application components
 * @param obj Specifies pointer to application base instance
 */
void topdax_startup(struct application *obj);

/**
 * Shuts down application components
 * @param obj Specifies pointer to application base instance
 */
void topdax_shutdown(struct application *obj);

/**
 * Activate application
 * @param obj Specifies pointer to application base instance
 */
void topdax_activate(struct application *obj);

/**
 * Close application window
 * @param obj Specifies pointer to topdax window
 */
void topdax_close_window(struct window_handler *obj, struct window *win);

/**
 * Runs the application
 * @param argc @a argc from main() function
 * @param argv @a argv from main() function
 * @return exit code
 */
int topdax_run(int argc, char **argv);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
