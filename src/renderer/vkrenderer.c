/**
 * @file
 * Vulkan renderer implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <renderer/vkrenderer.h>

/** Readable macro to placate compiler */
#define UNUSED(x) (void)(x)

int vkrenderer_init(struct vkrenderer *rdr)
{
	UNUSED(rdr);
	return 0;
}

void vkrenderer_terminate(struct vkrenderer *rdr)
{
	UNUSED(rdr);
}
