/**
 * @file
 * Test suite for logger
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdarg.h>
#include <stdio.h>

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "logger.h"

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL
vkGetInstanceProcAddr(VkInstance instance, const char *pName)
{
	return (PFN_vkVoidFunction) mock(instance, pName);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateDebugUtilsMessengerEXT(VkInstance instance,
			       const VkDebugUtilsMessengerCreateInfoEXT *
			       pCreateInfo,
			       const VkAllocationCallbacks * pAllocator,
			       VkDebugUtilsMessengerEXT * pMessenger)
{
	return (VkResult) mock(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
				VkDebugUtilsMessengerEXT messenger,
				const VkAllocationCallbacks * pAllocator)
{
	mock(instance, messenger, pAllocator);
}

int __wrap_fprintf(FILE * __restrict __stream, const char *__restrict __format,
		   ...)
{
	va_list args;
	va_start(args, __format);
	const char *arg1 = va_arg(args, const char *);
	const char *arg2 = va_arg(args, const char *);
	va_end(args);

	return (int)mock(__stream, __format, arg1, arg2);
}

Ensure(setup_debug_logger_initializes_messenger)
{
	VkInstance instance = NULL;
	expect(vkGetInstanceProcAddr,
	       when(pName,
		    is_equal_to_string("vkCreateDebugUtilsMessengerEXT")),
	       will_return(vkCreateDebugUtilsMessengerEXT));
	expect(vkCreateDebugUtilsMessengerEXT);
	setup_debug_logger(instance);
}

Ensure(destroy_debug_logger_destroys_messenger)
{
	VkInstance instance = NULL;
	expect(vkGetInstanceProcAddr,
	       when(pName,
		    is_equal_to_string("vkDestroyDebugUtilsMessengerEXT")),
	       will_return(vkDestroyDebugUtilsMessengerEXT));
	expect(vkDestroyDebugUtilsMessengerEXT);
	destroy_debug_logger(instance);
}

Ensure(logger_prints_messages_to_stderr)
{
	VkBool32 res;
	const VkDebugUtilsMessengerCallbackDataEXT CallbackData = {
		.sType =
		    VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
		.pNext = NULL,
		.flags = 0,
		.pMessageIdName = "MID",
		.messageIdNumber = 0,
		.pMessage = "Message",
		.queueLabelCount = 0,
		.pQueueLabels = NULL,
		.cmdBufLabelCount = 0,
		.pCmdBufLabels = NULL,
		.objectCount = 0,
		.pObjects = NULL
	};

	expect(__wrap_fprintf,
	       when(__stream, is_equal_to(stderr)),
	       when(__format, is_equal_to_string("%s: %s\n")),
	       when(arg1, is_equal_to_string("verbose: ")),
	       when(arg2, is_equal_to_string(CallbackData.pMessage)));
	res = vk_debug_print(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
			     VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
			     &CallbackData, NULL);
	assert_that(res, is_equal_to(VK_FALSE));

	expect(__wrap_fprintf,
	       when(__stream, is_equal_to(stderr)),
	       when(__format, is_equal_to_string("%s: %s\n")),
	       when(arg1, is_equal_to_string("info: ")),
	       when(arg2, is_equal_to_string(CallbackData.pMessage)));
	res = vk_debug_print(VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
			     VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
			     &CallbackData, NULL);
	assert_that(res, is_equal_to(VK_FALSE));

	expect(__wrap_fprintf,
	       when(__stream, is_equal_to(stderr)),
	       when(__format, is_equal_to_string("%s: %s\n")),
	       when(arg1, is_equal_to_string("warning: ")),
	       when(arg2, is_equal_to_string(CallbackData.pMessage)));
	res = vk_debug_print(VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
			     VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
			     &CallbackData, NULL);
	assert_that(res, is_equal_to(VK_FALSE));

	expect(__wrap_fprintf,
	       when(__stream, is_equal_to(stderr)),
	       when(__format, is_equal_to_string("%s: %s\n")),
	       when(arg1, is_equal_to_string("error: ")),
	       when(arg2, is_equal_to_string(CallbackData.pMessage)));
	res = vk_debug_print(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			     VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
			     &CallbackData, NULL);
	assert_that(res, is_equal_to(VK_FALSE));
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *suite = create_named_test_suite("Logger");
	add_test(suite, setup_debug_logger_initializes_messenger);
	add_test(suite, destroy_debug_logger_destroys_messenger);
	add_test(suite, logger_prints_messages_to_stderr);
	TestReporter *reporter = create_text_reporter();
	int exit_code = run_test_suite(suite, reporter);
	destroy_reporter(reporter);
	destroy_test_suite(suite);
	return exit_code;
}
