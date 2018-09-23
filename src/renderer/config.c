/**
 * @file
 * Vulkan renderer config selection implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <application/utils.h>
#include <renderer/vkrenderer.h>
#include <string.h>
#include "vkrenderer.h"

int find_queue_families(struct device_config *cfg, VkSurfaceKHR surface)
{
	VkQueueFamilyProperties fams[32];
	uint32_t nfams = ARRAY_SIZE(fams);
	VkBool32 present_support[ARRAY_SIZE(fams)];
	VkBool32 graphic_support[ARRAY_SIZE(fams)];
	vkGetPhysicalDeviceQueueFamilyProperties(cfg->phy, &nfams, fams);
	for (uint32_t i = 0; i < nfams; i++) {
		vkGetPhysicalDeviceSurfaceSupportKHR(cfg->phy, i, surface,
						     &present_support[i]);
		graphic_support[i] = fams[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;
	}

	/* Trying to find family that supports both graphics and presentation */
	for (uint32_t i = 0; i < nfams; i++) {
		//if (present_support[i] && graphic_support[i] && fams[i].queueCount >= 2) {
		if (present_support[i] && graphic_support[i]) {
			cfg->graph_family = i;
			cfg->present_family = i;
			return 1;
		}
	}

	/* Trying to find separate families for presentation and graphics
	 * support */
	for (uint32_t i = 0; i < nfams; i++) {
		if (present_support[i] && fams[i].queueCount > 0) {
			fams[i].queueCount -= 1;
			cfg->present_family = i;
			break;
		}
	}
	for (uint32_t i = 0; i < nfams; i++) {
		if (graphic_support[i] && fams[i].queueCount > 0) {
			fams[i].queueCount -= 1;
			cfg->graph_family = i;
			break;
		}
	}
	return (cfg->graph_family < nfams) && (cfg->present_family < nfams);
}

int choose_config(VkInstance instance, struct device_config *cfg,
		  VkSurfaceKHR surface)
{
	static const char *req_dev_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	VkPhysicalDevice phy[4];
	uint32_t nphy = ARRAY_SIZE(phy);
	VkResult result = vkEnumeratePhysicalDevices(instance, &nphy, phy);
	if (result != VK_SUCCESS || nphy == 0) {
		return 1;
	}
	for (size_t i = 0; i < nphy; i++) {
		cfg->phy = phy[i];
		memset(&cfg->features, 0, sizeof(VkPhysicalDeviceFeatures));
		cfg->extensions = req_dev_extensions;
		cfg->nextensions = ARRAY_SIZE(req_dev_extensions);
		if (find_queue_families(cfg, surface)) {
			return 0;
		}
	}
	return 1;
}
