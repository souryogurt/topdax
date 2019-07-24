/**
 * @file
 * Mock of config_swapchain module
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/mocks.h>

#include "config_swapchain.h"

int vkrenderer_configure_swapchain(struct vkrenderer *rdr)
{
	return (int)mock(rdr);
}
