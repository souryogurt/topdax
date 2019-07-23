/**
 * @file
 * Mock of phy_device module
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/mocks.h>

#include "phy_device.h"

void phy_device_init(struct phy_device *dev, VkPhysicalDevice phy)
{
	mock(dev, phy);
}

uint32_t phy_family_count(const struct phy_device *dev)
{
	return (uint32_t)mock(dev);
}

bool phy_family_can_graphics(const struct phy_device *dev, uint32_t fidx)
{
	return (bool)mock(dev, fidx);
}

bool phy_family_can_present(const struct phy_device *dev, uint32_t fidx,
			    VkSurfaceKHR srf)
{
	return (bool)mock(dev, fidx, srf);
}
