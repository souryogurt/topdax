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
#include "vkrenderer.h"
#include "phy_device.h"

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
 * Choose graphics and presentation families
 * @param rdr Specifies renderer to choose families for
 * @returns zero if indices are found, and non-zero otherwise
 */
static int vkrenderer_configure_families(struct vkrenderer *rdr)
{
	VkQueueFamilyProperties fams[32];
	struct phy_device dev = {
		.nfams = ARRAY_SIZE(fams),
		.fams = fams,
	};
	phy_device_init(&dev, rdr->phy);

	if (phy_find_universal_family(&dev, rdr->srf, &rdr->graphic)) {
		rdr->present = rdr->graphic;
		return 0;
	}
	if (!phy_find_graphic_family(&dev, &rdr->graphic)) {
		return -1;
	}
	if (!phy_find_present_family(&dev, rdr->srf, &rdr->present)) {
		return -1;
	}
	return 0;
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
