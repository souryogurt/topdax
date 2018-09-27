/**
 * @file
 * Vulkan renderer config selection implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <application/utils.h>
#include <string.h>
#include "vkrenderer.h"

/** Queue Family Properties */
struct family_properties {
	/** Number of queue families available */
	uint32_t count;
	/** Queue Family Properties returned from Vulkan */
	VkQueueFamilyProperties *fams;
	/** Flags specifying presentation support */
	VkBool32 *present;
	/** Flags specifying graphics support */
	VkBool32 *graphic;
};

/**
 * Selects queue families that supports graphics and presentation
 * @param props Specifies available family queues
 * @param graph Specifies pointer where store graphics family index
 * @param present Specifies pointer where store presentation family index
 * @returns zero if indices are found, and non-zero otherwise
 */
static int
select_families(const struct family_properties *props, uint32_t * graph,
		uint32_t * present)
{
	*graph = props->count;
	*present = props->count;
	for (uint32_t i = 0; i < props->count; i++) {
		if (props->present[i]) {
			*present = i;
			break;
		}
	}
	for (uint32_t i = 0; i < props->count; i++) {
		if (props->graphic[i]) {
			*graph = i;
			break;
		}
	}
	return (*graph >= props->count) || (*present >= props->count);
}

/**
 * Selects queue family that supports both graphics and presentation
 * @param props Specifies available family queues
 * @param graph Specifies pointer where store universal family index
 * @param present Specifies pointer where store universal family index
 * @returns zero if family is found, and non-zero otherwise
 */
static int
select_universal_families(const struct family_properties *props,
			  uint32_t * graph, uint32_t * present)
{
	for (uint32_t i = 0; i < props->count; i++) {
		if (props->present[i] && props->graphic[i]) {
			*graph = i;
			*present = i;
			return 0;
		}
	}
	return -1;
}

/**
 * Tests and fills family properties
 * @param phy Specifies physical device against of which to test properties
 * @param srf Specifies surface  against of which to test properties
 * @param fpr Specifies pointer to array of available family properties
 * @param fam Specifies family index to test
 */
static void
set_family_properties(VkPhysicalDevice phy, VkSurfaceKHR srf,
		      struct family_properties *fpr, uint32_t fam)
{
	VkBool32 has_queue = fpr->fams[fam].queueCount > 0;
	vkGetPhysicalDeviceSurfaceSupportKHR(phy, fam, srf, &fpr->present[fam]);
	fpr->graphic[fam] = fpr->fams[fam].queueFlags & VK_QUEUE_GRAPHICS_BIT;
	fpr->present[fam] = has_queue && fpr->present[fam];
	fpr->graphic[fam] = has_queue && fpr->graphic[fam];
}

/**
 * Choose graphics and presentation families
 * @param rdr Specifies renderer to choose families for
 * @returns zero if indices are found, and non-zero otherwise
 */
static int vkrenderer_configure_families(struct vkrenderer *rdr)
{
	VkQueueFamilyProperties families[32];
	VkBool32 is_present[ARRAY_SIZE(families)];
	VkBool32 is_graphic[ARRAY_SIZE(families)];
	struct family_properties props = {
		.count = ARRAY_SIZE(families),
		.fams = families,
		.present = is_present,
		.graphic = is_graphic,
	};
	vkGetPhysicalDeviceQueueFamilyProperties(rdr->phy, &props.count,
						 props.fams);
	for (uint32_t i = 0; i < props.count; i++) {
		set_family_properties(rdr->phy, rdr->srf, &props, i);
	}
	if (!select_universal_families(&props, &rdr->graphic, &rdr->present)) {
		return 0;
	}
	return select_families(&props, &rdr->graphic, &rdr->present);
}

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
	return 0;
}

int vkrenderer_configure(struct vkrenderer *rdr, VkInstance instance)
{
	VkPhysicalDevice phy[4];
	uint32_t nphy = ARRAY_SIZE(phy);
	if (vkEnumeratePhysicalDevices(instance, &nphy, phy) != VK_SUCCESS) {
		return -1;
	}
	for (size_t i = 0; i < nphy; i++) {
		rdr->phy = phy[i];
		if (!vkrenderer_configure_device(rdr))
			return 0;
	}
	return -1;
}
