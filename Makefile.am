ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include
@CODE_COVERAGE_RULES@
dist_doc_DATA = README.md LICENSE

noinst_LIBRARIES = libtopdax.a
libtopdax_a_SOURCES = src/topdax.c
libtopdax_a_SOURCES += include/topdax/topdax.h
libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += librunloop.a
librunloop_a_SOURCES = include/topdax/application.h
librunloop_a_SOURCES += include/GLFW/runloop.h
librunloop_a_SOURCES += src/GLFW/runloop.c
librunloop_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += libwindow.a
libwindow_a_SOURCES = include/topdax/application.h
libwindow_a_SOURCES += include/GLFW/window.h
libwindow_a_SOURCES += src/GLFW/window.c
libwindow_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += libvkrenderer.a
libvkrenderer_a_SOURCES = include/topdax/vkrenderer.h
libvkrenderer_a_SOURCES += src/vkrenderer.c
libvkrenderer_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

bin_PROGRAMS = topdax
topdax_SOURCES = src/GLFW/entry.c
topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_LDADD = librunloop.a libtopdax.a libwindow.a libvkrenderer.a $(CODE_COVERAGE_LIBS)

if HAVE_CGREEN
TESTS = topdax_suite
check_PROGRAMS = topdax_suite
topdax_suite_SOURCES = tests/topdax_suite.c
topdax_suite_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_suite_LDADD = libtopdax.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += glfw_suite
check_PROGRAMS += glfw_suite
glfw_suite_SOURCES = tests/glfw_suite.c
glfw_suite_CFLAGS = $(CODE_COVERAGE_CFLAGS)
glfw_suite_LDADD = librunloop.a libwindow.a -lcgreen $(CODE_COVERAGE_LIBS)
glfw_suite_LDFLAGS = -Wl,--wrap=argp_parse

TESTS += vkrenderer_suite
check_PROGRAMS += vkrenderer_suite
vkrenderer_suite_SOURCES = tests/vkrenderer_suite.c
vkrenderer_suite_CFLAGS = $(CODE_COVERAGE_CFLAGS)
vkrenderer_suite_LDADD = libvkrenderer.a -lcgreen $(CODE_COVERAGE_LIBS)
endif
