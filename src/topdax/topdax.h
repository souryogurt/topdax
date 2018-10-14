#ifndef TOPDAX_TOPDAX_H
#define TOPDAX_TOPDAX_H

#include <vulkan/vulkan_core.h>
#include "window.h"

struct runloop;

/** Runloop operations */
struct runloop_ops {
	/** Specifies pointer to function to quit from runloop */
	void (*quit)(struct runloop *);
};

/** Abstract runloop interface */
struct runloop {
	/** Specifies runloop implementation */
	const struct runloop_ops *ops;
};

/** Topdax Application */
struct topdax {
	/** Vulkan instance */
	VkInstance vk;
	/** Pointer to mainloop */
	struct runloop *mainloop;
	/** Main window */
	struct topdax_window window;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/** Version string */
extern const char *const g_app_version;

/** Name and email of person responsible for issues */
extern const char *const g_app_bug_address;

/** Application description */
extern const char *const g_app_description;

/**
 * Starts up application components
 * @param loop Specfies loop this application is running on
 * @returns zero on success or non-zero otherwise
 */
int application_startup(struct runloop *loop);

/**
 * Shuts down application components
 */
void application_shutdown(void);

/**
 * Runs the application
 * @param argc Specifies @a argc from main() function
 * @param argv Specifies @a argv from main() function
 * @return exit code
 */
int glfw_runloop_run(int argc, char **argv);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
