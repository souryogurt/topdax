/**
 * @file
 * Topdax application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "topdax.h"

/** Topdax application instance */
static struct topdax app;

/** Version string */
const char *const application_version = PACKAGE_STRING;

/** Name and email of person responsible for issues */
const char *const application_bug_address = PACKAGE_BUGREPORT;

/** Application description */
const char *const application_description =
    "The program that renders triangle using Vulkan API";

static int vulkan_initialize(VkInstance * vk)
{
	const VkInstanceCreateInfo create_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = NULL,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = 0,
		.ppEnabledExtensionNames = NULL,
	};
	VkResult result = vkCreateInstance(&create_info, NULL, vk);
	return result != VK_SUCCESS;
}

void application_startup(struct runloop *loop)
{
	app.mainloop = loop;
	vulkan_initialize(&app.vk);
	topdax_window_init(&app.window, &app);
}

void application_shutdown(void)
{
	topdax_window_destroy(&app.window);
	vkDestroyInstance(app.vk, NULL);
}
