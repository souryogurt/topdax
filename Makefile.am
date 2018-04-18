ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include
@CODE_COVERAGE_RULES@
dist_doc_DATA = README.md LICENSE

noinst_LIBRARIES = libtopdax.a
libtopdax_a_SOURCES = src/topdax.c
libtopdax_a_SOURCES += include/topdax/topdax.h
libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += libapplication.a
libapplication_a_SOURCES = include/topdax/application.h
libapplication_a_SOURCES += src/glfw_application.c
libapplication_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

bin_PROGRAMS = topdax
topdax_SOURCES = src/entry.c
topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_LDADD = libtopdax.a libapplication.a $(CODE_COVERAGE_LIBS)

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
glfw_suite_LDADD = libapplication.a -lcgreen $(CODE_COVERAGE_LIBS)
glfw_suite_LDFLAGS = -Wl,--wrap=argp_parse
endif
