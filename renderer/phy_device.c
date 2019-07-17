/**
 * @file
 * Physical device properties chooser
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "phy_device.h"

void phy_device_init(struct phy_device *dev, VkPhysicalDevice phy)
{
	vkGetPhysicalDeviceQueueFamilyProperties(phy, &dev->nfams, dev->fams);
	dev->phy = phy;
}

bool phy_find_graphic_family(const struct phy_device *dev, uint32_t *index)
{
	for (*index = 0; *index < dev->nfams; ++(*index)) {
		const VkQueueFamilyProperties *props = &dev->fams[*index];
		if (props->queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			return true;
		}
	}
	return false;
}

bool phy_find_present_family(const struct phy_device *dev, VkSurfaceKHR srf,
			     uint32_t *index)
{
	VkBool32 present = VK_FALSE;
	for (*index = 0; *index < dev->nfams; ++(*index)) {
		vkGetPhysicalDeviceSurfaceSupportKHR(dev->phy, *index, srf,
						     &present);
		if (present == VK_TRUE) {
			return true;
		}
	}
	return false;
}

bool phy_find_universal_family(const struct phy_device *dev, VkSurfaceKHR srf,
			       uint32_t *index)
{
	VkBool32 graphic = VK_FALSE;
	VkBool32 present = VK_FALSE;
	for (*index = 0; *index < dev->nfams; ++(*index)) {
		const VkQueueFamilyProperties *props = &dev->fams[*index];
		graphic = (props->queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0;
		vkGetPhysicalDeviceSurfaceSupportKHR(dev->phy, *index, srf,
						     &present);
		if (present == VK_TRUE && graphic == VK_TRUE) {
			return true;
		}
	}
	return false;
}
