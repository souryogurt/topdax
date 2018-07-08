bin_PROGRAMS += topdax
topdax_SOURCES = src/GLFW/entry.c
topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_LDADD = librunloop.a libtopdax.a libwindow.a libvkrenderer.a $(CODE_COVERAGE_LIBS)

noinst_LIBRARIES += libtopdax.a
libtopdax_a_SOURCES = src/topdax/topdax.c
libtopdax_a_SOURCES += src/topdax/window.c
libtopdax_a_SOURCES += include/topdax/topdax.h
libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += topdax_test
check_PROGRAMS += topdax_test
topdax_test_SOURCES = src/topdax/topdax_test.c
topdax_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_test_LDADD = libtopdax.a -lcgreen $(CODE_COVERAGE_LIBS)
endif
