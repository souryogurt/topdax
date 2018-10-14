#ifndef TOPDAX_TOPDAX_H
#define TOPDAX_TOPDAX_H

#include <vulkan/vulkan_core.h>
#include "window.h"

/** Topdax Application */
struct topdax {
	/** Vulkan instance */
	VkInstance vk;
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
 * @returns zero on success or non-zero otherwise
 */
int application_startup(void);

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
int topdax_main(int argc, char **argv);

/**
 * Post quit message
 */
void topdax_quit(void);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
