ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include
@CODE_COVERAGE_RULES@
EXTRA_DIST = README.md LICENSE

noinst_LIBRARIES = libtopdax.a
libtopdax_a_SOURCES = src/topdax.c
libtopdax_a_SOURCES += include/topdax/topdax.h
libtopdax_a_SOURCES += include/topdax/application.h
libtopdax_a_SOURCES += src/glfw_application.c
libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS) $(GLFW_CFLAGS)

bin_PROGRAMS = topdax
topdax_SOURCES = src/entry.c
topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS) $(GLFW_CFLAGS)
topdax_LDADD = libtopdax.a $(CODE_COVERAGE_LIBS) $(GLFW_LIBS)

if HAVE_CGREEN
TESTS = topdax_suite
check_PROGRAMS = topdax_suite
topdax_suite_SOURCES = tests/topdax_suite.c
topdax_suite_CFLAGS = $(CGREEN_CFLAGS) $(CODE_COVERAGE_CFLAGS) $(GLFW_CFLAGS)
topdax_suite_LDADD = libtopdax.a $(CGREEN_LIBS) $(CODE_COVERAGE_LIBS)

TESTS += glfw_suite
check_PROGRAMS += glfw_suite
glfw_suite_SOURCES = tests/glfw_suite.c
glfw_suite_CFLAGS = $(CGREEN_CFLAGS) $(CODE_COVERAGE_CFLAGS) $(GLFW_CFLAGS)
glfw_suite_LDADD = libtopdax.a $(CGREEN_LIBS) $(CODE_COVERAGE_LIBS)
endif
