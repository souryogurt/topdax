/**
 * @file
 * Debug logger
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "logger.h"

#include <string.h>
#include <stdio.h>

/** Readable macro to placate compiler */
#define UNUSED(x) (void)(x)

static VkDebugUtilsMessengerEXT vk_debug_messenger;

VKAPI_ATTR VkBool32 VKAPI_CALL
vk_debug_print(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	       VkDebugUtilsMessageTypeFlagsEXT messageType,
	       const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
	       void *pUserData)
{
	UNUSED(pUserData);
	UNUSED(messageType);
	const char *prefix = "unknown: ";
	if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		prefix = "verbose: ";
	} else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		prefix = "info: ";
	} else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		prefix = "warning: ";
	} else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		prefix = "error: ";
	}
	fprintf(stderr, "%s: %s\n", prefix, pCallbackData->pMessage);
	return VK_FALSE;
}

void setup_debug_logger(VkInstance vk)
{
	PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
	pfnVkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)
	    vkGetInstanceProcAddr(vk, "vkCreateDebugUtilsMessengerEXT");
	const VkDebugUtilsMessengerCreateInfoEXT msgr_info = {
		.sType =
		    VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.pNext = NULL,
		.flags = 0,
		.messageSeverity =
#if 0
		    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
#endif
		    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = vk_debug_print,
		.pUserData = NULL
	};
	pfnVkCreateDebugUtilsMessengerEXT(vk, &msgr_info, NULL,
					  &vk_debug_messenger);
}

void destroy_debug_logger(VkInstance vk)
{
	PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestoyDebugUtilsMessengerEXT;
	pfnVkDestoyDebugUtilsMessengerEXT =
	    (PFN_vkDestroyDebugUtilsMessengerEXT)
	    vkGetInstanceProcAddr(vk, "vkDestroyDebugUtilsMessengerEXT");
	pfnVkDestoyDebugUtilsMessengerEXT(vk, vk_debug_messenger, NULL);
}
