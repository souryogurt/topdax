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
 * @returns non-zero if indices are found, and zero otherwise
 */
static int
select_families(const struct family_properties *props, uint32_t * graph,
		uint32_t * present)
{
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
	return (*graph < props->count) && (*present < props->count);
}

/**
 * Selects queue family that supports both graphics and presentation
 * @param props Specifies available family queues
 * @param graph Specifies pointer where store universal family index
 * @param present Specifies pointer where store universal family index
 * @returns non-zero if family is found, and zero otherwise
 */
static int
select_universal_families(const struct family_properties *props,
			  uint32_t * graph, uint32_t * present)
{
	for (uint32_t i = 0; i < props->count; i++) {
		if (props->present[i] && props->graphic[i]) {
			*graph = i;
			*present = i;
			return 1;
		}
	}
	return 0;
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
 * @param phy Specifies physical device to choose families from
 * @param srf Specifies surface for presentation family
 * @param graph Specifies pointer where store graphics family index
 * @param present Specifies pointer where store presentation family index
 * @returns non-zero if indices are found, and zero otherwise
 */
static int
choose_families(VkPhysicalDevice phy, VkSurfaceKHR srf, uint32_t * graph,
		uint32_t * present)
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
	vkGetPhysicalDeviceQueueFamilyProperties(phy, &props.count, props.fams);
	for (uint32_t i = 0; i < props.count; i++) {
		set_family_properties(phy, srf, &props, i);
	}
	if (select_universal_families(&props, graph, present)) {
		return 1;
	}
	return select_families(&props, graph, present);
}

int choose_config(VkInstance instance, struct device_config *cfg,
		  VkSurfaceKHR surface)
{
	static const char *const req_dev_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	VkPhysicalDevice phy[4];
	uint32_t nphy = ARRAY_SIZE(phy);
	if (vkEnumeratePhysicalDevices(instance, &nphy, phy) != VK_SUCCESS) {
		return 1;
	}
	for (size_t i = 0; i < nphy; i++) {
		cfg->phy = phy[i];
		memset(&cfg->features, 0, sizeof(VkPhysicalDeviceFeatures));
		cfg->extensions = req_dev_extensions;
		cfg->nextensions = ARRAY_SIZE(req_dev_extensions);
		if (choose_families(phy[i], surface, &cfg->graph_family,
				    &cfg->present_family)) {
			return 0;
		}
	}
	return 1;
}
