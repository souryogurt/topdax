#ifndef RENDERER_VKRENDERER_CONFIG_H
#define RENDERER_VKRENDERER_CONFIG_H

#include <renderer/vkrenderer.h>

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

/**
 * Choose graphics and presentation families
 * @param rdr Specifies renderer to choose families for
 * @returns zero if indices are found, and non-zero otherwise
 */
int vkrenderer_configure_families(struct vkrenderer *rdr);

/**
 * Choose swapchain parameters
 * @param rdr Specifies renderer to choose swapchain parameters for
 * @returns zero if parameters are found, and non-zero otherwise
 */
int vkrenderer_configure_swapchain(struct vkrenderer *rdr);

#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#endif
