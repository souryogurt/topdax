#ifndef TOPDAX_LOGGER_H
#define TOPDAX_LOGGER_H

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Setup global debug logger for Vulkan instance
 * @param vkn Vulkan instance handle on which logger must be set
 */
void setup_debug_logger(VkInstance vkn);

/**
 * Destroy global debug logger on Vulkan instance
 * @param vkn Vulkan instance handle on which logger must be destroyed
 */
void destroy_debug_logger(VkInstance vkn);

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
