/**
 * @file
 * Test suite for vkrenderer
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include "topdax/vkrenderer.h"

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo *
						pCreateInfo,
						const VkAllocationCallbacks *
						pAllocator,
						VkInstance * pInstance)
{
	return mock(pCreateInfo, pAllocator, pInstance);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance,
					     const VkAllocationCallbacks *
					     pAllocator)
{
	mock(instance, pAllocator);
}

Ensure(vkrenderer_init_returns_zero_on_success)
{
	struct vkrenderer vkr;
	expect(vkCreateInstance);
	int error = vkrenderer_init(&vkr);
	assert_that(error, is_equal_to(0));
}

Ensure(vkrenderer_terminate_destroys_all_resources)
{
	struct vkrenderer vkr;
	expect(vkDestroyInstance);
	vkrenderer_terminate(&vkr);
}

int main(int argc, char **argv)
{
	(void)(argc);
	(void)(argv);
	TestSuite *vkr = create_named_test_suite("VKRenderer");
	add_test(vkr, vkrenderer_init_returns_zero_on_success);
	add_test(vkr, vkrenderer_terminate_destroys_all_resources);
	return run_test_suite(vkr, create_text_reporter());
}
