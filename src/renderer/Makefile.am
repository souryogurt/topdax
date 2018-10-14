noinst_LIBRARIES += lib/libvkrenderer.a
lib_libvkrenderer_a_SOURCES = include/renderer/vkrenderer.h
lib_libvkrenderer_a_SOURCES += src/renderer/vkrenderer.h
lib_libvkrenderer_a_SOURCES += src/renderer/vkrenderer.c
lib_libvkrenderer_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += lib/libvkframe.a
lib_libvkframe_a_SOURCES = include/renderer/vkrenderer.h
lib_libvkframe_a_SOURCES += src/renderer/vkframe.c
lib_libvkframe_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += lib/libvkconfig.a
lib_libvkconfig_a_SOURCES = src/renderer/vkrenderer.h
lib_libvkconfig_a_SOURCES += src/renderer/config.c
lib_libvkconfig_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += lib/libvkconfig_families.a
lib_libvkconfig_families_a_SOURCES = src/renderer/vkrenderer.h
lib_libvkconfig_families_a_SOURCES += src/renderer/config_families.c
lib_libvkconfig_families_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += lib/libvkconfig_swapchain.a
lib_libvkconfig_swapchain_a_SOURCES = src/renderer/vkrenderer.h
lib_libvkconfig_swapchain_a_SOURCES += src/renderer/config_swapchain.c
lib_libvkconfig_swapchain_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += bin/vkrenderer_test
check_PROGRAMS += bin/vkrenderer_test
bin_vkrenderer_test_SOURCES = src/renderer/vkrenderer_test.c
bin_vkrenderer_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_vkrenderer_test_LDADD = lib/libvkrenderer.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += bin/vkframe_test
check_PROGRAMS += bin/vkframe_test
bin_vkframe_test_SOURCES = src/renderer/vkframe_test.c
bin_vkframe_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_vkframe_test_LDADD = lib/libvkframe.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += bin/config_test
check_PROGRAMS += bin/config_test
bin_config_test_SOURCES = src/renderer/config_test.c
bin_config_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_config_test_LDADD = lib/libvkconfig.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += bin/config_families_test
check_PROGRAMS += bin/config_families_test
bin_config_families_test_SOURCES = src/renderer/config_families_test.c
bin_config_families_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_config_families_test_LDADD = lib/libvkconfig_families.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += bin/config_swapchain_test
check_PROGRAMS += bin/config_swapchain_test
bin_config_swapchain_test_SOURCES = src/renderer/config_swapchain_test.c
bin_config_swapchain_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_config_swapchain_test_LDADD = lib/libvkconfig_swapchain.a -lcgreen $(CODE_COVERAGE_LIBS)
endif
