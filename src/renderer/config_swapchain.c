/**
 * @file
 * Vulkan renderer swapchain selection implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <application/utils.h>
#include <string.h>
#include "vkrenderer.h"

/**
 * Select suitable surface format from available
 * @param fmts Specifies array of available surface formats on device
 * @param nfmts Specifies number of elements in @a fmts array
 * @param fmt Specifies pointer to memory where selected format must be stored
 * @returns zero if format is found, and non-zero otherwise
 */
static int select_surface_format(const VkSurfaceFormatKHR * fmts,
				 uint32_t nfmts, VkSurfaceFormatKHR * fmt)
{
	if (nfmts == 0)
		return -1;
	memcpy(fmt, &fmts[0], sizeof(VkSurfaceFormatKHR));
	return 0;
}

/**
 * Choose surface format
 * @param rdr Specifies renderer to choose surface format for
 * @returns zero if format is found, and non-zero otherwise
 */
static int vkrenderer_configure_surface_format(struct vkrenderer *rdr)
{
	VkSurfaceFormatKHR fmts[256];
	uint32_t nfmts = ARRAY_SIZE(fmts);

	VkResult result;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(rdr->phy, rdr->srf,
						      &nfmts, fmts);
	if (result != VK_SUCCESS)
		return -1;

	return select_surface_format(fmts, nfmts, &rdr->srf_format);
}

/**
 * Select suitable presentation mode from available
 * @param modes Specifies array of available presentation modes on device
 * @param nmodes Specifies number of elements in @a modes array
 * @param mode Specifies pointer to memory where selected mode must be stored
 * @returns zero if mode is found, and non-zero otherwise
 */
static int select_surface_present_mode(const VkPresentModeKHR * modes,
				       uint32_t nmodes, VkPresentModeKHR * mode)
{
	if (nmodes == 0)
		return -1;
	memcpy(mode, &modes[0], sizeof(VkPresentModeKHR));
	return 0;
}

/**
 * Choose surface presentation parameters
 * @param rdr Specifies renderer to choose surface presentation parameters for
 * @returns zero if parameters are found, and non-zero otherwise
 */
static int vkrenderer_configure_surface_present_mode(struct vkrenderer *rdr)
{
	VkPresentModeKHR modes[32];
	uint32_t nmodes = ARRAY_SIZE(modes);
	VkResult result;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(rdr->phy, rdr->srf,
							   &nmodes, modes);
	if (result != VK_SUCCESS)
		return -1;
	return select_surface_present_mode(modes, nmodes, &rdr->srf_mode);
}

/**
 * Choose swapchain parameters
 * @param rdr Specifies renderer to choose swapchain parameters for
 * @returns zero if parameters are found, and non-zero otherwise
 */
int vkrenderer_configure_swapchain(struct vkrenderer *rdr)
{
	VkResult result;
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(rdr->phy, rdr->srf,
							   &rdr->srf_caps);
	if (result != VK_SUCCESS)
		return -1;
	if (vkrenderer_configure_surface_format(rdr))
		return -1;
	if (vkrenderer_configure_surface_present_mode(rdr))
		return -1;
	return 0;
}
