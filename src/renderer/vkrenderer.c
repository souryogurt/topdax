/**
 * @file
 * Vulkan renderer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <application/utils.h>
#include <renderer/vkrenderer.h>
#include <string.h>
#include "vkrenderer.h"

int vkrenderer_init(struct vkrenderer *rdr, VkInstance instance,
		    VkSurfaceKHR surface)
{
	struct device_config cfg;
	if (choose_config(instance, &cfg, surface)) {
		return 1;
	}

	float queue_priorities[2] = { 1.0f, 1.0f };
	VkDeviceQueueCreateInfo qinfos[2] = {
		{
		 .queueFamilyIndex = cfg.graph_family,
		 },
		{
		 .queueFamilyIndex = cfg.present_family,
		 },
	};
	uint32_t nqinfos = (cfg.graph_family == cfg.present_family) ? 1 : 2;
	for (size_t i = 0; i < nqinfos; i++) {
		VkDeviceQueueCreateInfo *info = &qinfos[i];
		info->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info->pNext = NULL;
		info->flags = 0;
		info->queueFamilyIndex = cfg.graph_family;
		info->queueCount = 1;
		info->pQueuePriorities = queue_priorities;
	}
	VkDeviceCreateInfo dev_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.queueCreateInfoCount = nqinfos,
		.pQueueCreateInfos = qinfos,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = cfg.nextensions,
		.ppEnabledExtensionNames = cfg.extensions,
		.pEnabledFeatures = &cfg.features,
	};
	VkResult err = vkCreateDevice(cfg.phy, &dev_info, NULL, &rdr->device);
	if (err != VK_SUCCESS) {
		return 1;
	}
	vkGetDeviceQueue(rdr->device, cfg.graph_family, 0,
			 &rdr->graphics_queue);
	vkGetDeviceQueue(rdr->device, cfg.present_family, 0,
			 &rdr->present_queue);
	return 0;
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	vkDestroyDevice(rdr->device, NULL);
}
