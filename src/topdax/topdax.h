#ifndef TOPDAX_TOPDAX_H
#define TOPDAX_TOPDAX_H

#include <vulkan/vulkan_core.h>
#include "window.h"

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

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
