/**
 * @file
 * Vulkan renderer config selection implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include <string.h>

#include <vulkan/vulkan_core.h>
#include <renderer/vkrenderer.h>
#include "vkrenderer.h"

/**
 * Configure device features
 * @param rdr Specifies renderer to configure
 */
static void vkrenderer_configure_features(struct vkrenderer *rdr)
{
	/* We are not enabling any features, so no need to check for them */
	memset(&rdr->features, 0, sizeof(VkPhysicalDeviceFeatures));
}

/**
 * Configure device extensions
 * @param rdr Specifies renderer to configure
 */
static void vkrenderer_configure_extensions(struct vkrenderer *rdr)
{
	static const char *const req_dev_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	/* TODO: check that required extension is really supported */
	rdr->extensions = req_dev_extensions;
	rdr->nextensions = ARRAY_SIZE(req_dev_extensions);
}

/**
 * Configure renderer on specific physical device
 * @param rdr Specifies renderer to configure
 * @returns zero on success, or non-zero otherwise
 */
static int vkrenderer_configure_device(struct vkrenderer *rdr)
{
	vkrenderer_configure_features(rdr);
	vkrenderer_configure_extensions(rdr);
	if (vkrenderer_configure_families(rdr))
		return -1;
	if (vkrenderer_configure_swapchain(rdr))
		return -1;
	return 0;
}

int vkrenderer_configure(struct vkrenderer *rdr, VkInstance instance)
{
	VkPhysicalDevice phy[4];
	uint32_t nphy = ARRAY_SIZE(phy);
	if (vkEnumeratePhysicalDevices(instance, &nphy, phy) != VK_SUCCESS) {
		return -1;
	}
	for (size_t i = 0; i < nphy; ++i) {
		rdr->phy = phy[i];
		if (!vkrenderer_configure_device(rdr))
			return 0;
	}
	return -1;
}
