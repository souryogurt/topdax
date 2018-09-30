noinst_LIBRARIES += libvkrenderer.a
libvkrenderer_a_SOURCES = include/renderer/vkrenderer.h
libvkrenderer_a_SOURCES += src/renderer/vkrenderer.h
libvkrenderer_a_SOURCES += src/renderer/vkrenderer.c
libvkrenderer_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += libvkconfig.a
libvkconfig_a_SOURCES = src/renderer/vkrenderer.h
libvkconfig_a_SOURCES += src/renderer/config.c
libvkconfig_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += libvkconfig_families.a
libvkconfig_families_a_SOURCES = src/renderer/vkrenderer.h
libvkconfig_families_a_SOURCES += src/renderer/config_families.c
libvkconfig_families_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += libvkconfig_swapchain.a
libvkconfig_swapchain_a_SOURCES = src/renderer/vkrenderer.h
libvkconfig_swapchain_a_SOURCES += src/renderer/config_swapchain.c
libvkconfig_swapchain_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += vkrenderer_test
check_PROGRAMS += vkrenderer_test
vkrenderer_test_SOURCES = src/renderer/vkrenderer_test.c
vkrenderer_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
vkrenderer_test_LDADD = libvkrenderer.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += config_test
check_PROGRAMS += config_test
config_test_SOURCES = src/renderer/config_test.c
config_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
config_test_LDADD = libvkconfig.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += config_families_test
check_PROGRAMS += config_families_test
config_families_test_SOURCES = src/renderer/config_families_test.c
config_families_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
config_families_test_LDADD = libvkconfig_families.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += config_swapchain_test
check_PROGRAMS += config_swapchain_test
config_swapchain_test_SOURCES = src/renderer/config_swapchain_test.c
config_swapchain_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
config_swapchain_test_LDADD = libvkconfig_swapchain.a -lcgreen $(CODE_COVERAGE_LIBS)
endif
