#ifndef TOPDAX_LOGGER_H
#define TOPDAX_LOGGER_H

#include <vulkan/vulkan.h>

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Setup global debug logger for Vulkan instance
 * @param instance Vulkan instance handle on which logger must be set
 */
void setup_debug_logger(VkInstance instance);

/**
 * Destroy global debug logger on Vulkan instance
 * @param instance Vulkan instance handle on which logger must be destroyed
 */
void destroy_debug_logger(VkInstance instance);

/**
 * Prints Vulkan errors to stderr
 */
VKAPI_ATTR VkBool32 VKAPI_CALL
vk_debug_print(VkDebugUtilsMessageSeverityFlagBitsEXT sev,
	       VkDebugUtilsMessageTypeFlagsEXT messageType,
	       const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
	       void *pUserData);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif

#endif
