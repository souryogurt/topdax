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

/** Topdax application information */
static const VkApplicationInfo topdax_app_info = {
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pApplicationName = PACKAGE,
	.applicationVersion = VK_MAKE_VERSION(0, 3, 0),
	.pEngineName = PACKAGE,
	.engineVersion = VK_MAKE_VERSION(0, 3, 0),
	.apiVersion = VK_API_VERSION_1_1
};

static VkResult vk_instance_create(VkInstance * vk)
{
	uint32_t exts_count;
	const char **exts = glfwGetRequiredInstanceExtensions(&exts_count);

	const VkInstanceCreateInfo vk_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &topdax_app_info,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = exts_count,
		.ppEnabledExtensionNames = exts
	};
	return vkCreateInstance(&vk_info, NULL, vk);
}

int application_startup(struct runloop *loop)
{
	if (!glfwInit())
		return 1;
	app.mainloop = loop;

	if (vk_instance_create(&app.vk) != VK_SUCCESS) {
		return 1;
	}

	topdax_window_init(&app.window, &app);
	return 0;
}

void application_shutdown(void)
{
	topdax_window_destroy(&app.window);
	vkDestroyInstance(app.vk, NULL);
	glfwTerminate();
}
