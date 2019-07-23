#ifndef RENDERER_PHY_DEVICE_H
#define RENDERER_PHY_DEVICE_H

#include <stdint.h>
#include <stdbool.h>

#include <vulkan/vulkan_core.h>

/** Physical device properties */
struct phy_device {
	VkPhysicalDevice phy;
	uint32_t nfams;
	VkQueueFamilyProperties *fams;
};

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Initialize physical device properties
 * @param dev Specifies physical device to initialize
 */
void phy_device_init(struct phy_device *dev, VkPhysicalDevice phy);

/**
 * Query number of families supported by device
 * @param dev Specifies physical device_to query
 * @returns number of families supported
 */
uint32_t phy_family_count(const struct phy_device *dev);

/**
 * Query whether a queue family in physical device supports graphics operations
 * @param dev Specifies physical device to query
 * @param fidx Specifies index of queue family to query
 * @returns true if supports, or false otherwise
 */
bool phy_family_can_graphics(const struct phy_device *dev, uint32_t fidx);

/**
 * Query whether a queue family in physical device supports presentation to a
 * given surface.
 * @param dev Specifies physical device to query
 * @param fidx Specifies index of queue family to query
 * @param srf Specifies surface to query
 * @returns true if supports, or false otherwise
 */
bool phy_family_can_present(const struct phy_device *dev, uint32_t fidx,
			    VkSurfaceKHR srf);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
