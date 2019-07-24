noinst_LTLIBRARIES += renderer/libvkrenderer.la
renderer_libvkrenderer_la_SOURCES = renderer/vkrenderer.h\
				    renderer/vkrenderer.c\
				    renderer/vkswapchain.h\
				    renderer/vkswapchain.c\
				    renderer/vkframe.h\
				    renderer/vkframe.c\
				    renderer/config.h\
				    renderer/config.c\
				    renderer/config_swapchain.h\
				    renderer/config_swapchain.c\
				    renderer/phy_device.h\
				    renderer/phy_device.c

if HAVE_CGREEN
TESTS += renderer/vkrenderer_test
check_PROGRAMS += renderer/vkrenderer_test
renderer_vkrenderer_test_SOURCES = renderer/vkrenderer_test.c\
				   renderer/libvulkan_mock.c\
				   renderer/config_swapchain_mock.c\
				   renderer/config_mock.c\
				   renderer/vkswapchain_mock.c
renderer_vkrenderer_test_LDADD = renderer/libvkrenderer.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/vkswapchain_test
check_PROGRAMS += renderer/vkswapchain_test
renderer_vkswapchain_test_SOURCES = renderer/vkswapchain_test.c\
				    renderer/libvulkan_mock.c\
				    renderer/vkframe_mock.c
renderer_vkswapchain_test_LDADD = renderer/libvkrenderer.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/vkframe_test
check_PROGRAMS += renderer/vkframe_test
renderer_vkframe_test_SOURCES = renderer/vkframe_test.c\
				renderer/libvulkan_mock.c
renderer_vkframe_test_LDADD = renderer/libvkrenderer.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/config_test
check_PROGRAMS += renderer/config_test
renderer_config_test_SOURCES = renderer/config_test.c\
			       renderer/phy_device_mock.c\
			       renderer/libvulkan_mock.c\
			       renderer/config_swapchain_mock.c
renderer_config_test_LDADD = renderer/libvkrenderer.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/config_swapchain_test
check_PROGRAMS += renderer/config_swapchain_test
renderer_config_swapchain_test_SOURCES = renderer/config_swapchain_test.c\
					 renderer/libvulkan_mock.c
renderer_config_swapchain_test_LDADD = renderer/libvkrenderer.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/phy_device_test
check_PROGRAMS += renderer/phy_device_test
renderer_phy_device_test_SOURCES = renderer/phy_device_test.c\
				   renderer/libvulkan_mock.c
renderer_phy_device_test_LDADD = renderer/libvkrenderer.la -lcgreen $(CODE_COVERAGE_LIBS)
endif
