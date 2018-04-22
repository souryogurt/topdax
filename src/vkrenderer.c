/**
 * @file
 * Vulkan renderer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "topdax/vkrenderer.h"

int vkrenderer_init(struct vkrenderer *rdr)
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
	VkResult result = vkCreateInstance(&create_info, NULL, &rdr->instance);
	return result != VK_SUCCESS;
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	vkDestroyInstance(rdr->instance, NULL);
}
