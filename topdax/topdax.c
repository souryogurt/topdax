/**
 * @file
 * Topdax application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <argp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <threads.h>

#include <stdatomic.h>
#include <stdbool.h>

#include <GLFW/glfw3.h>
#include "logger.h"
#include "topdax.h"
#include "window.h"
#include <renderer/vkrenderer.h>
#include <vulkan/vulkan_core.h>

/** Vulkan instance */
static VkInstance vkn;
/** Main window */
static struct topdax_window window;

/** Renderer implementation */
static struct vkrenderer renderer;

/** Arguments parser */
static struct argp argp;

/** Vulkan compatible application version */
#define VK_APP_VERSION \
	VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH)

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
static int get_vk_extensions(const char **extensions, uint32_t *size)
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
static VkResult vk_instance_create(VkInstance *instance)
{
	const char *extensions[100];
	uint32_t nextensions = ARRAY_SIZE(extensions);
	if (get_vk_extensions(extensions, &nextensions) != 0) {
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	VkInstanceCreateInfo vk_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &topdax_app_info,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = nextensions,
		.ppEnabledExtensionNames = extensions
	};
	return vkCreateInstance(&vk_info, NULL, instance);
}

struct thread_data {
	atomic_bool is_running;
};

static int render_main(void *data)
{
	struct thread_data *thrd = (struct thread_data *)data;
	do {
		vkrenderer_render(&renderer);
	} while (thrd->is_running);
	return 0;
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
	if (topdax_window_init(&window, vkn)) {
		return EXIT_FAILURE;
	}
	if (vkrenderer_init(&renderer, vkn, window.surface)) {
		return EXIT_FAILURE;
	}
	struct thread_data rdr_data;
	atomic_init(&rdr_data.is_running, true);
	thrd_t render_thread;
	thrd_create(&render_thread, render_main, &rdr_data);
	while (!glfwWindowShouldClose(window.id)) {
		glfwWaitEvents();
	}
	rdr_data.is_running = false;
	thrd_join(render_thread, NULL);
	vkrenderer_terminate(&renderer);
	topdax_window_destroy(&window);
#ifndef NDEBUG
	destroy_debug_logger(vkn);
#endif
	vkDestroyInstance(vkn, NULL);
	glfwTerminate();
	return EXIT_SUCCESS;
}
