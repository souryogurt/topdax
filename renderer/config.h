#ifndef RENDERER_CONFIG_H
#define RENDERER_CONFIG_H

struct vkrenderer;

#include <vulkan/vulkan_core.h>

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/**
 * Configures renderer on Vulkan instance
 * @param rdr Specifies pointer to renderer to configure
 * @param instance Specifies Vulkan instance
 * @returns zero on success, or non-zero otherwise
 */
int vkrenderer_configure(struct vkrenderer *rdr, VkInstance instance);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
