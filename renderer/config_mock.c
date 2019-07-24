/**
 * @file
 * Mock of config module
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/mocks.h>

#include "config.h"

int vkrenderer_configure(struct vkrenderer *rdr, VkInstance instance)
{
	return (int)mock(rdr, instance);
}
