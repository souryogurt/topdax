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

#include <GLFW/glfw3.h>
#include "logger.h"
#include "topdax.h"
#include "window.h"
#include <renderer/vkrenderer.h>

/** Vulkan instance */
static VkInstance vkn;
/** Main window */
static struct topdax_window window;

/** Arguments parser */
static struct argp argp;

/** Vulkan compatible application version */
#define VK_APP_VERSION VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH)

/** Topdax application information */
static const VkApplicationInfo topdax_app_info = {
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pApplicationName = PACKAGE,
	.applicationVersion = VK_APP_VERSION,
	.pEngineName = PACKAGE,
	.engineVersion = VK_APP_VERSION,
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

int application_main(int argc, char **argv)
{
	argp_program_version = PACKAGE_STRING;
	argp_program_bug_address = PACKAGE_BUGREPORT;
	argp.doc = "The program that renders triangle using Vulkan API";
	if (argp_parse(&argp, argc, argv, 0, NULL, NULL))
		return EXIT_FAILURE;

	if (!glfwInit())
		return EXIT_FAILURE;

	if (vk_instance_create(&vkn) != VK_SUCCESS) {
		return EXIT_FAILURE;
	}
#ifndef NDEBUG
	setup_debug_logger(vkn);
#endif
	topdax_window_init(&window, vkn);
	while (!glfwWindowShouldClose(window.id)) {
		glfwPollEvents();
		vkrenderer_render(&window.renderer);
	}
	topdax_window_destroy(&window);
#ifndef NDEBUG
	destroy_debug_logger(vkn);
#endif
	vkDestroyInstance(vkn, NULL);
	glfwTerminate();
	return EXIT_SUCCESS;
}
