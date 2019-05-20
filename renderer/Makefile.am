noinst_LTLIBRARIES += renderer/libvkrenderer.la
renderer_libvkrenderer_la_SOURCES = renderer/vkrenderer.h\
				    renderer/vkrenderer.c

noinst_LTLIBRARIES += renderer/libvkswapchain.la
renderer_libvkswapchain_la_SOURCES = renderer/vkswapchain.h\
				     renderer/vkswapchain.c

noinst_LTLIBRARIES += renderer/libvkframe.la
renderer_libvkframe_la_SOURCES = renderer/vkframe.h\
				 renderer/vkframe.c

noinst_LTLIBRARIES += renderer/libvkconfig.la
renderer_libvkconfig_la_SOURCES = renderer/vkrenderer.h\
				 renderer/config.c

noinst_LTLIBRARIES += renderer/libvkconfig_families.la
renderer_libvkconfig_families_la_SOURCES = renderer/vkrenderer.h\
					   renderer/config_families.c

noinst_LTLIBRARIES += renderer/libvkconfig_swapchain.la
renderer_libvkconfig_swapchain_la_SOURCES = renderer/vkrenderer.h\
					    renderer/config_swapchain.c

if HAVE_CGREEN
TESTS += renderer/vkrenderer_test
check_PROGRAMS += renderer/vkrenderer_test
renderer_vkrenderer_test_SOURCES = renderer/vkrenderer_test.c
renderer_vkrenderer_test_LDADD = renderer/libvkrenderer.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/vkswapchain_test
check_PROGRAMS += renderer/vkswapchain_test
renderer_vkswapchain_test_SOURCES = renderer/vkswapchain_test.c
renderer_vkswapchain_test_LDADD = renderer/libvkswapchain.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/vkframe_test
check_PROGRAMS += renderer/vkframe_test
renderer_vkframe_test_SOURCES = renderer/vkframe_test.c
renderer_vkframe_test_LDADD = renderer/libvkframe.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/config_test
check_PROGRAMS += renderer/config_test
renderer_config_test_SOURCES = renderer/config_test.c
renderer_config_test_LDADD = renderer/libvkconfig.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/config_families_test
check_PROGRAMS += renderer/config_families_test
renderer_config_families_test_SOURCES = renderer/config_families_test.c
renderer_config_families_test_LDADD = renderer/libvkconfig_families.la -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/config_swapchain_test
check_PROGRAMS += renderer/config_swapchain_test
renderer_config_swapchain_test_SOURCES = renderer/config_swapchain_test.c
renderer_config_swapchain_test_LDADD = renderer/libvkconfig_swapchain.la -lcgreen $(CODE_COVERAGE_LIBS)
endif
