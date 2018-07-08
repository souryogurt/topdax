ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include
@CODE_COVERAGE_RULES@
dist_doc_DATA = README.md LICENSE
TESTS =
check_PROGRAMS =


bin_PROGRAMS = topdax
topdax_SOURCES = src/GLFW/entry.c
topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_LDADD = librunloop.a libtopdax.a libwindow.a libvkrenderer.a $(CODE_COVERAGE_LIBS)

noinst_LIBRARIES = libtopdax.a
libtopdax_a_SOURCES = src/topdax/topdax.c
libtopdax_a_SOURCES += include/topdax/topdax.h
libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += topdax_test
check_PROGRAMS += topdax_test
topdax_test_SOURCES = src/topdax/topdax_test.c
topdax_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_test_LDADD = libtopdax.a -lcgreen $(CODE_COVERAGE_LIBS)
endif


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

if HAVE_CGREEN
TESTS += glfw_test
check_PROGRAMS += glfw_test
glfw_test_SOURCES = src/GLFW/glfw_test.c
glfw_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
glfw_test_LDADD = librunloop.a libwindow.a -lcgreen $(CODE_COVERAGE_LIBS)
glfw_test_LDFLAGS = -Wl,--wrap=argp_parse
endif


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
