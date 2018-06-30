#ifndef TOPDAX_H
#define TOPDAX_H
#include "topdax/application.h"
#include <vulkan/vulkan.h>

/** Readable macro to placate compiler */
#define UNUSED(x) (void)(x)

#ifdef __GNUC__
#define member_type(type, member) __typeof__ (((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

#define container_of(ptr, type, member) ((type *)( \
    (char *)(void*)(member_type(type, member) *){ ptr } - offsetof(type, member)))

struct vkrenderer;

/** Topdax Application */
struct topdax {
	/** Vulkan instance */
	VkInstance vk;
	/** Pointer to renderer */
	struct vkrenderer *rdr;
	/** Implements base application */
	struct application app;
	/** Main window */
	struct window win;
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
void topdax_close_window(struct window *obj);

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
