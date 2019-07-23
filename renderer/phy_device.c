/**
 * @file
 * Physical device properties iterator
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

uint32_t phy_family_count(const struct phy_device *dev)
{
	return dev->nfams;
}

bool phy_family_can_graphics(const struct phy_device *dev, uint32_t fidx)
{
	return (dev->fams[fidx].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0;
}

bool phy_family_can_present(const struct phy_device *dev, uint32_t fidx,
			    VkSurfaceKHR srf)
{
	VkBool32 ret = VK_FALSE;
	vkGetPhysicalDeviceSurfaceSupportKHR(dev->phy, fidx, srf, &ret);
	return ret != VK_FALSE;
}
