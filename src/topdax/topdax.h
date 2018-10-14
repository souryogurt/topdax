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

/**
 * Application entrypoint
 * @param argc Specifies @a argc from main() function
 * @param argv Specifies @a argv from main() function
 * @return exit code
 */
int application_main(int argc, char **argv);

/**
 * Post quit message
 */
void application_quit(void);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
