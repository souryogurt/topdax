noinst_LIBRARIES += renderer/libvkrenderer.a
renderer_libvkrenderer_a_SOURCES = renderer/vkrenderer.h
renderer_libvkrenderer_a_SOURCES += renderer/vkrenderer.h
renderer_libvkrenderer_a_SOURCES += renderer/vkrenderer.c
renderer_libvkrenderer_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += renderer/libvkswapchain.a
renderer_libvkswapchain_a_SOURCES = renderer/vkswapchain.h
renderer_libvkswapchain_a_SOURCES += renderer/vkswapchain.c
renderer_libvkswapchain_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += renderer/libvkframe.a
renderer_libvkframe_a_SOURCES = renderer/vkframe.h
renderer_libvkframe_a_SOURCES += renderer/vkframe.c
renderer_libvkframe_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += renderer/libvkconfig.a
renderer_libvkconfig_a_SOURCES = renderer/vkrenderer.h
renderer_libvkconfig_a_SOURCES += renderer/config.c
renderer_libvkconfig_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += renderer/libvkconfig_families.a
renderer_libvkconfig_families_a_SOURCES = renderer/vkrenderer.h
renderer_libvkconfig_families_a_SOURCES += renderer/config_families.c
renderer_libvkconfig_families_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += renderer/libvkconfig_swapchain.a
renderer_libvkconfig_swapchain_a_SOURCES = renderer/vkrenderer.h
renderer_libvkconfig_swapchain_a_SOURCES += renderer/config_swapchain.c
renderer_libvkconfig_swapchain_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += renderer/vkrenderer_test
check_PROGRAMS += renderer/vkrenderer_test
renderer_vkrenderer_test_SOURCES = renderer/vkrenderer_test.c
renderer_vkrenderer_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
renderer_vkrenderer_test_LDADD = renderer/libvkrenderer.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/vkswapchain_test
check_PROGRAMS += renderer/vkswapchain_test
renderer_vkswapchain_test_SOURCES = renderer/vkswapchain_test.c
renderer_vkswapchain_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
renderer_vkswapchain_test_LDADD = renderer/libvkswapchain.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/vkframe_test
check_PROGRAMS += renderer/vkframe_test
renderer_vkframe_test_SOURCES = renderer/vkframe_test.c
renderer_vkframe_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
renderer_vkframe_test_LDADD = renderer/libvkframe.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/config_test
check_PROGRAMS += renderer/config_test
renderer_config_test_SOURCES = renderer/config_test.c
renderer_config_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
renderer_config_test_LDADD = renderer/libvkconfig.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/config_families_test
check_PROGRAMS += renderer/config_families_test
renderer_config_families_test_SOURCES = renderer/config_families_test.c
renderer_config_families_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
renderer_config_families_test_LDADD = renderer/libvkconfig_families.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += renderer/config_swapchain_test
check_PROGRAMS += renderer/config_swapchain_test
renderer_config_swapchain_test_SOURCES = renderer/config_swapchain_test.c
renderer_config_swapchain_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
renderer_config_swapchain_test_LDADD = renderer/libvkconfig_swapchain.a -lcgreen $(CODE_COVERAGE_LIBS)
endif
