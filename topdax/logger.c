/**
 * @file
 * Debug logger
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "logger.h"
#include <stdio.h>

/** Readable macro to placate compiler */
#define UNUSED(x) (void)(x)

static VkDebugUtilsMessengerEXT vk_debug_messenger;

VKAPI_ATTR VkBool32 VKAPI_CALL
vk_debug_print(VkDebugUtilsMessageSeverityFlagBitsEXT sev,
	       VkDebugUtilsMessageTypeFlagsEXT messageType,
	       const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
	       void *pUserData)
{
	UNUSED(pUserData);
	UNUSED(messageType);
	const char *prefix = "unknown: ";
	int isverbose = sev & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
	int isinfo = sev & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
	int iswarning = sev & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
	int iserror = sev & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	if (isverbose) {
		prefix = "verbose: ";
	} else if (isinfo) {
		prefix = "info: ";
	} else if (iswarning) {
		prefix = "warning: ";
	} else if (iserror) {
		prefix = "error: ";
	}
	fprintf(stderr, "%s: %s\n", prefix, pCallbackData->pMessage);
	return VK_FALSE;
}

void setup_debug_logger(VkInstance vkn)
{
	PFN_vkCreateDebugUtilsMessengerEXT create_debug_msgr;
	create_debug_msgr =
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			vkn, "vkCreateDebugUtilsMessengerEXT");
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
		.pfnUserCallback = &vk_debug_print,
		.pUserData = NULL
	};
	create_debug_msgr(vkn, &msgr_info, NULL, &vk_debug_messenger);
}

void destroy_debug_logger(VkInstance vkn)
{
	PFN_vkDestroyDebugUtilsMessengerEXT destroy_debug_msgr;
	destroy_debug_msgr =
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			vkn, "vkDestroyDebugUtilsMessengerEXT");
	destroy_debug_msgr(vkn, vk_debug_messenger, NULL);
}
