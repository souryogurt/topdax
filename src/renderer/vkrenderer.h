#ifndef RENDERER_VKRENDERER_CONFIG_H
#define RENDERER_VKRENDERER_CONFIG_H

#include <vulkan/vulkan.h>

struct device_config {
	VkPhysicalDevice phy;
	VkSurfaceKHR srf;
	VkPhysicalDeviceFeatures features;
	const char **extensions;
	uint32_t nextensions;
	uint32_t graph_family;
	uint32_t present_family;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

int find_queue_families(struct device_config *cfg, VkSurfaceKHR surface);

int choose_config(VkInstance instance, struct device_config *cfg,
		  VkSurfaceKHR surface);
#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
