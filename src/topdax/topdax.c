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

/** Topdax Vulkan Instance information */
static const VkInstanceCreateInfo topdax_vk_info = {
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO
};

int application_startup(struct runloop *loop)
{
	if (!glfwInit())
		return 1;
	app.mainloop = loop;
	if (vkCreateInstance(&topdax_vk_info, NULL, &app.vk) != VK_SUCCESS) {
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
