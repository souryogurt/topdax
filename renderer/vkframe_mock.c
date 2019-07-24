/**
 * @file
 * Mock of vkframe module
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/mocks.h>

#include "vkframe.h"

VkResult vkframe_init(struct vkframe *frame, const VkRenderPass rpass,
		      const struct vkrenderer *rdr, const VkImage image)
{
	return (VkResult)mock(frame, rpass, rdr, image);
}

void vkframe_destroy(const struct vkframe *frame, const VkDevice device)
{
	mock(frame, device);
}
