/**
 * @file
 * Mock of vkswapchain module
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/mocks.h>

#include "vkswapchain.h"

int vkswapchain_init(struct vkswapchain *swc, const struct vkrenderer *rdr,
		     const VkSwapchainKHR old_swc)
{
	return (int)mock(swc, rdr, old_swc);
}

void vkswapchain_terminate(const struct vkswapchain *swc, VkDevice dev)
{
	mock(swc, dev);
}

VkResult vkswapchain_render(const struct vkswapchain *swc,
			    const struct vkrenderer *rdr)
{
	return (VkResult)mock(swc, rdr);
}
