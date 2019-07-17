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

/** Initialize physical device properties
 * @param dev Specifies physical device to initialize
 */
void phy_device_init(struct phy_device *dev, VkPhysicalDevice phy);

/** Find queue family that supports graphic operations
 * @param dev Specifies physical device to choose family from
 * @param index Specifies pointer where store family index
 * @returns true if found, and false otherwise
 */
bool phy_find_graphic_family(const struct phy_device *dev, uint32_t *index);

/** Find queue family that supports presentation operations
 * @param dev Specifies physical device to choose family from
 * @param srf Specifies surface the family must be compatible with
 * @param index Specifies pointer where store family index
 * @returns true if found, and false otherwise
 */
bool phy_find_present_family(const struct phy_device *dev, VkSurfaceKHR srf,
			     uint32_t *index);

/**
 * Find queue family that supports both graphics and presentation
 * @param dev Specifies physical device to choose family from
 * @param srf Specifies surface the family must be compatible with
 * @param index Specifies pointer where store family index
 * @returns true if found, and false otherwise
 */
bool phy_find_universal_family(const struct phy_device *dev, VkSurfaceKHR srf,
			       uint32_t *index);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
