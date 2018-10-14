/**
 * @file
 * Topdax application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <argp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <application/utils.h>
#include <GLFW/glfw3.h>
#include "logger.h"
#include "topdax.h"
#include "window.h"

/** Topdax application instance */
static struct topdax app;

/** Version string */
const char *argp_program_version = PACKAGE_STRING;

/** Name and email of person responsible for issues */
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

/** Arguments parser */
static struct argp argp;

/** zero if application is running, non-zero otherwise */
static int g_app_must_quit;

/** Topdax application information */
static const VkApplicationInfo topdax_app_info = {
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pApplicationName = PACKAGE,
	.applicationVersion = VK_MAKE_VERSION(0, 3, 0),
	.pEngineName = PACKAGE,
	.engineVersion = VK_MAKE_VERSION(0, 3, 0),
	.apiVersion = VK_API_VERSION_1_1
};

/**
 * Returns array of extensions required to create Vulkan instance
 * @param extensions Specifies pointer to destination array into which required
 *                   extensions will be copied
 * @param size Specifies pointer to size of @a extensions array
 * @returns zero on success or non-zero otherwise
 */
static int get_vk_extensions(const char **extensions, uint32_t * size)
{
	uint32_t wsi_count;
	const char **wsi_exts = glfwGetRequiredInstanceExtensions(&wsi_count);
	uint32_t all_count = wsi_count;
#ifndef NDEBUG
	const char *const debug_exts[] = { "VK_EXT_debug_utils" };
	all_count += ARRAY_SIZE(debug_exts);
#endif

	if (all_count <= *size) {
		memcpy(extensions, wsi_exts, sizeof(wsi_exts[0]) * wsi_count);

#ifndef NDEBUG
		memcpy(&extensions[wsi_count], debug_exts, sizeof(debug_exts));
#endif
		*size = all_count;
		return 0;
	}
	*size = all_count;
	return 1;
}

/**
 * Create Vulkan instance suitable for topdax application rendering
 * @param instance Specifies pointer to Vulkan handle in which the resulting
 *                 instance is returned
 * @returns VK_SUCCESS on success
 */
static VkResult vk_instance_create(VkInstance * instance)
{
	const char *extensions[100];
	VkInstanceCreateInfo vk_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &topdax_app_info,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = ARRAY_SIZE(extensions),
		.ppEnabledExtensionNames = extensions
	};
	if (get_vk_extensions(extensions, &vk_info.enabledExtensionCount) == 0) {
		return vkCreateInstance(&vk_info, NULL, instance);
	}
	return VK_ERROR_INITIALIZATION_FAILED;
}

void application_quit(void)
{
	g_app_must_quit++;
}

int application_main(int argc, char **argv)
{
	argp.doc = "The program that renders triangle using Vulkan API";
	if (argp_parse(&argp, argc, argv, 0, NULL, NULL))
		return EXIT_FAILURE;

	if (!glfwInit())
		return EXIT_FAILURE;

	if (vk_instance_create(&app.vk) != VK_SUCCESS) {
		return EXIT_FAILURE;
	}
#ifndef NDEBUG
	setup_debug_logger(app.vk);
#endif
	topdax_window_init(&app.window, &app);
	do {
		glfwWaitEvents();
	} while (!g_app_must_quit);
	topdax_window_destroy(&app.window);
#ifndef NDEBUG
	destroy_debug_logger(app.vk);
#endif
	vkDestroyInstance(app.vk, NULL);
	glfwTerminate();
	return EXIT_SUCCESS;
}
