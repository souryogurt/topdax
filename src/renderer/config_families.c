/**
 * @file
 * Vulkan renderer families selection implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

#include <application/utils.h>
#include <vulkan/vulkan_core.h>
#include <renderer/vkrenderer.h>

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
	for (uint32_t i = 0; i < props->count; ++i) {
		if (props->present[i]) {
			*present = i;
			break;
		}
	}
	for (uint32_t i = 0; i < props->count; ++i) {
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
	for (uint32_t i = 0; i < props->count; ++i) {
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
int vkrenderer_configure_families(struct vkrenderer *rdr)
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
	for (uint32_t i = 0; i < props.count; ++i) {
		set_family_properties(rdr->phy, rdr->srf, &props, i);
	}
	if (!select_universal_families(&props, &rdr->graphic, &rdr->present)) {
		return 0;
	}
	return select_families(&props, &rdr->graphic, &rdr->present);
}
