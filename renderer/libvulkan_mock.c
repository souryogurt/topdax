/**
 * @file
 * Mock of libvulkan module
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/mocks.h>

#include <vulkan/vulkan_core.h>

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
	VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
	VkSurfaceCapabilitiesKHR *caps)
{
	return (VkResult)mock(physicalDevice, surface, caps);
}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(
	VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
	uint32_t *pSurfaceFormatCount, VkSurfaceFormatKHR *pSurfaceFormats)
{
	return (VkResult)mock(physicalDevice, surface, pSurfaceFormatCount,
			      pSurfaceFormats);
}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(
	VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
	uint32_t *pPresentModeCount, VkPresentModeKHR *pPresentModes)
{
	return (VkResult)mock(physicalDevice, surface, pPresentModeCount,
			      pPresentModes);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkEnumeratePhysicalDevices(VkInstance instance, uint32_t *pPhysicalDeviceCount,
			   VkPhysicalDevice *pPhysicalDevices)
{
	return (VkResult)mock(instance, pPhysicalDeviceCount, pPhysicalDevices);
}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(
	VkPhysicalDevice physicalDevice, uint32_t *pQueueFamilyCount,
	VkQueueFamilyProperties *pQueueFamily)
{
	mock(physicalDevice, pQueueFamilyCount, pQueueFamily);
}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(
	VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex,
	VkSurfaceKHR surface, VkBool32 *pSupported)
{
	return (VkResult)mock(physicalDevice, queueFamilyIndex, surface,
			      pSupported);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkCreateImageView(VkDevice device, const VkImageViewCreateInfo *pCreateInfo,
		  const VkAllocationCallbacks *pAllocator, VkImageView *pView)
{
	return (VkResult)mock(device, pCreateInfo, pAllocator, pView);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyImageView(VkDevice device, VkImageView imageView,
		   const VkAllocationCallbacks *pAllocator)
{
	mock(device, imageView, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateFramebuffer(
	VkDevice device, const VkFramebufferCreateInfo *pCreateInfo,
	const VkAllocationCallbacks *pAllocator, VkFramebuffer *pFramebuffer)
{
	return (VkResult)mock(device, pCreateInfo, pAllocator, pFramebuffer);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer,
		     const VkAllocationCallbacks *pAllocator)
{
	mock(device, framebuffer, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vkAllocateCommandBuffers(
	VkDevice device, const VkCommandBufferAllocateInfo *pAllocateInfo,
	VkCommandBuffer *pCommandBuffers)
{
	return (VkResult)mock(device, pAllocateInfo, pCommandBuffers);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkBeginCommandBuffer(VkCommandBuffer commandBuffer,
		     const VkCommandBufferBeginInfo *pBeginInfo)
{
	return (VkResult)mock(commandBuffer, pBeginInfo);
}

VKAPI_ATTR void VKAPI_CALL
vkCmdBeginRenderPass(VkCommandBuffer commandBuffer,
		     const VkRenderPassBeginInfo *pRenderPassBegin,
		     VkSubpassContents contents)
{
	mock(commandBuffer, pRenderPassBegin, contents);
}

VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass(VkCommandBuffer commandBuffer)
{
	mock(commandBuffer);
}

VKAPI_ATTR VkResult VKAPI_CALL vkEndCommandBuffer(VkCommandBuffer commandBuffer)
{
	return (VkResult)mock(commandBuffer);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(
	VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo *pCreateInfo,
	const VkAllocationCallbacks *pAllocator, VkDevice *pDevice)
{
	return (VkResult)mock(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue(VkDevice device,
					    uint32_t queueFamilyIndex,
					    uint32_t queueIndex,
					    VkQueue *pQueue)
{
	mock(device, queueFamilyIndex, queueIndex, pQueue);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyDevice(VkDevice device, const VkAllocationCallbacks *pAllocator)
{
	mock(device, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateCommandPool(
	VkDevice device, const VkCommandPoolCreateInfo *pCreateInfo,
	const VkAllocationCallbacks *pAllocator, VkCommandPool *pCommandPool)
{
	return (VkResult)mock(device, pCreateInfo, pAllocator, pCommandPool);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool,
		     const VkAllocationCallbacks *pAllocator)
{
	mock(device, commandPool, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass(
	VkDevice device, const VkRenderPassCreateInfo *pCreateInfo,
	const VkAllocationCallbacks *pAllocator, VkRenderPass *pRenderPass)
{
	return (VkResult)mock(device, pCreateInfo, pAllocator, pRenderPass);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass,
		    const VkAllocationCallbacks *pAllocator)
{
	mock(device, renderPass, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vkDeviceWaitIdle(VkDevice device)
{
	return (VkResult)mock(device);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateSwapchainKHR(
	VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
	const VkAllocationCallbacks *pAllocator, VkSwapchainKHR *pSwapchain)
{
	return (VkResult)mock(device, pCreateInfo, pAllocator, pSwapchain);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain,
		      const VkAllocationCallbacks *pAllocator)
{
	mock(device, swapchain, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainImagesKHR(
	VkDevice device, VkSwapchainKHR swapchain,
	uint32_t *pSwapchainImageCount, VkImage *pSwapchainImages)
{
	return (VkResult)mock(device, swapchain, pSwapchainImageCount,
			      pSwapchainImages);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateSemaphore(
	VkDevice device, const VkSemaphoreCreateInfo *pCreateInfo,
	const VkAllocationCallbacks *pAllocator, VkSemaphore *pSemaphore)
{
	return (VkResult)mock(device, pCreateInfo, pAllocator, pSemaphore);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroySemaphore(VkDevice device, VkSemaphore semaphore,
		   const VkAllocationCallbacks *pAllocator)
{
	mock(device, semaphore, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImageKHR(
	VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout,
	VkSemaphore semaphore, VkFence fence, uint32_t *pImageIndex)
{
	return (VkResult)mock(device, swapchain, timeout, semaphore, fence,
			      pImageIndex);
}

VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit(VkQueue queue,
					     uint32_t submitCount,
					     const VkSubmitInfo *pSubmits,
					     VkFence fence)
{
	return (VkResult)mock(queue, submitCount, pSubmits, fence);
}

VKAPI_ATTR VkResult VKAPI_CALL
vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR *pPresentInfo)
{
	return (VkResult)mock(queue, pPresentInfo);
}
