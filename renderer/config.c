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
 * Setup device features
 * @param rdr Specifies renderer to setup
 * @param dev Specifies physical device to use
 * @returns true on success, false otherwise
 */
static bool vkrenderer_setup_features(struct vkrenderer *rdr,
				      const struct phy_device *dev)
{
	/* We are not enabling any features, so no need to check for them */
	memset(&rdr->features, 0, sizeof(VkPhysicalDeviceFeatures));
	return true;
}

/**
 * Setup device extensions
 * @param rdr Specifies renderer to setup
 * @param dev Specifies physical device to use
 * @returns true on success, false otherwise
 */
static bool vkrenderer_setup_extensions(struct vkrenderer *rdr,
					const struct phy_device *dev)
{
	/* TODO: check that required extension is really supported */
	static const char *const req_dev_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	rdr->extensions = req_dev_extensions;
	rdr->nextensions = ARRAY_SIZE(req_dev_extensions);
	return true;
}

/**
 * Setup device queue family that supports both graphics and presentation
 * @param rdr Specifies renderer to setup
 * @param dev Specifies physical device to use
 * @param srf Specifies surface will be used for presentation
 * @returns true on success, false otherwise
 */
static bool vkrenderer_setup_universal_queue(struct vkrenderer *rdr,
					     const struct phy_device *dev,
					     VkSurfaceKHR srf)
{
	uint32_t nfams = phy_family_count(dev);
	for (uint32_t fidx = 0; fidx < nfams; ++fidx) {
		if (phy_family_can_graphics(dev, fidx) &&
		    phy_family_can_present(dev, fidx, srf)) {
			rdr->present = rdr->graphic = fidx;
			return true;
		}
	}
	return false;
}

/**
 * Setup separate device queue families that supports graphics and presentation
 * @param rdr Specifies renderer to setup
 * @param dev Specifies physical device to use
 * @param srf Specifies surface will be used for presentation
 * @returns true on success, false otherwise
 */
static bool vkrenderer_setup_separate_queues(struct vkrenderer *rdr,
					     const struct phy_device *dev,
					     VkSurfaceKHR srf)
{
	uint32_t nfams = phy_family_count(dev);
	rdr->graphic = rdr->present = nfams;
	for (uint32_t fidx = 0; fidx < nfams; ++fidx) {
		if (phy_family_can_graphics(dev, fidx)) {
			rdr->graphic = fidx;
			break;
		}
	}
	for (uint32_t fidx = 0; fidx < nfams; ++fidx) {
		if (phy_family_can_present(dev, fidx, srf)) {
			rdr->present = fidx;
			break;
		}
	}
	return ((rdr->graphic < nfams) && (rdr->present < nfams));
}

/**
 * Setup device queue families
 * @param rdr Specifies renderer to setup
 * @param dev Specifies physical device to use
 * @param srf Specifies surface will be used for presentation
 * @returns true on success, false otherwise
 */
static bool vkrenderer_setup_queues(struct vkrenderer *rdr,
				    const struct phy_device *dev,
				    VkSurfaceKHR srf)
{
	if (vkrenderer_setup_universal_queue(rdr, dev, srf)) {
		return true;
	}
	return vkrenderer_setup_separate_queues(rdr, dev, srf);
}

/**
 * Setup device queue families
 * @param rdr Specifies renderer to setup
 * @param dev Specifies physical device to use
 * @returns true on success, false otherwise
 */
static bool vkrenderer_setup_device(struct vkrenderer *rdr,
				    const struct phy_device *dev)
{
	if (!vkrenderer_setup_features(rdr, dev))
		return false;
	if (!vkrenderer_setup_extensions(rdr, dev))
		return false;
	if (!vkrenderer_setup_queues(rdr, dev, rdr->srf))
		return false;
	if (vkrenderer_configure_swapchain(rdr))
		return false;
	return true;
}

int vkrenderer_configure(struct vkrenderer *rdr, VkInstance instance)
{
	VkPhysicalDevice phy[4];
	uint32_t nphy = ARRAY_SIZE(phy);
	if (vkEnumeratePhysicalDevices(instance, &nphy, phy) != VK_SUCCESS) {
		return -1;
	}

	VkQueueFamilyProperties fams[32];
	struct phy_device dev = {
		.nfams = ARRAY_SIZE(fams),
		.fams = fams,
	};
	for (size_t i = 0; i < nphy; ++i) {
		rdr->phy = phy[i];
		phy_device_init(&dev, phy[i]);
		if (vkrenderer_setup_device(rdr, &dev))
			return 0;
	}
	return -1;
}
