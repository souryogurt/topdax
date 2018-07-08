noinst_LIBRARIES += libvkrenderer.a
libvkrenderer_a_SOURCES = include/renderer/vkrenderer.h
libvkrenderer_a_SOURCES += src/renderer/vkrenderer.c
libvkrenderer_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += vkrenderer_test
check_PROGRAMS += vkrenderer_test
vkrenderer_test_SOURCES = src/renderer/vkrenderer_test.c
vkrenderer_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
vkrenderer_test_LDADD = libvkrenderer.a -lcgreen $(CODE_COVERAGE_LIBS)
endif
