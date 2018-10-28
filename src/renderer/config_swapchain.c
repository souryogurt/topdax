/**
 * @file
 * Vulkan renderer swapchain selection implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include <string.h>

#include <application/utils.h>
#include <vulkan/vulkan_core.h>
#include <renderer/vkrenderer.h>

/** Find BGRA_UNORM format for surface
 * @param fmts Specifies array of available surface formats on device
 * @param nfmts Specifies number of elements in @a fmts array
 * @param fmt Specifies pointer to memory where selected format must be stored
 * @returns zero if format is found, and non-zero otherwise
 */
static int select_simple_surface_format(const VkSurfaceFormatKHR * fmts,
					uint32_t nfmts,
					VkSurfaceFormatKHR * fmt)
{
	for (uint32_t i = 0; i < nfmts; ++i) {
		if (fmts[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
		    fmts[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			memcpy(fmt, &fmts[i], sizeof(VkSurfaceFormatKHR));
			return 0;
		}
	}
	return -1;
}

/**
 * Select suitable surface format from available
 * @param fmts Specifies array of available surface formats on device
 * @param nfmts Specifies number of elements in @a fmts array
 * @param fmt Specifies pointer to memory where selected format must be stored
 */
static void select_surface_format(const VkSurfaceFormatKHR * fmts,
				  uint32_t nfmts, VkSurfaceFormatKHR * fmt)
{
	if (nfmts == 1 && fmts[0].format == VK_FORMAT_UNDEFINED) {
		fmt->format = VK_FORMAT_B8G8R8A8_UNORM;
		fmt->colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		return;
	}
	if (!select_simple_surface_format(fmts, nfmts, fmt))
		return;
	memcpy(fmt, &fmts[0], sizeof(VkSurfaceFormatKHR));
}

int vkrenderer_configure_surface_format(struct vkrenderer *rdr)
{
	VkSurfaceFormatKHR fmts[256];
	uint32_t nfmts = ARRAY_SIZE(fmts);

	VkResult result;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(rdr->phy, rdr->srf,
						      &nfmts, fmts);
	if (result != VK_SUCCESS || nfmts == 0)
		return -1;

	select_surface_format(fmts, nfmts, &rdr->srf_format);
	return 0;
}

int vkrenderer_configure_surface_present_mode(struct vkrenderer *rdr)
{
	VkPresentModeKHR modes[32];
	uint32_t nmodes = ARRAY_SIZE(modes);
	VkResult result;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(rdr->phy, rdr->srf,
							   &nmodes, modes);
	if (result != VK_SUCCESS || nmodes == 0)
		return -1;
	rdr->srf_mode = VK_PRESENT_MODE_FIFO_KHR;
	return 0;
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
