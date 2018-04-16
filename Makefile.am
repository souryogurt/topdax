ACLOCAL_AMFLAGS = -I m4
@CODE_COVERAGE_RULES@
dist_doc_DATA = README.md LICENSE

noinst_LIBRARIES = topdax/libtopdax.a
topdax_libtopdax_a_SOURCES = topdax/topdax.c
topdax_libtopdax_a_SOURCES += topdax/topdax.h
topdax_libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += topdax/libapplication.a
topdax_libapplication_a_SOURCES = topdax/application.h
topdax_libapplication_a_SOURCES += topdax/glfw_application.c
topdax_libapplication_a_CFLAGS = $(CODE_COVERAGE_CFLAGS) $(GLFW_CFLAGS)

bin_PROGRAMS = topdax/topdax
topdax_topdax_SOURCES = topdax/entry.c
topdax_topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS) $(GLFW_CFLAGS)
topdax_topdax_LDADD = topdax/libtopdax.a topdax/libapplication.a $(CODE_COVERAGE_LIBS) $(GLFW_LIBS)

if HAVE_CGREEN
TESTS = tests/topdax_suite
check_PROGRAMS = tests/topdax_suite
tests_topdax_suite_SOURCES = tests/topdax_suite.c
tests_topdax_suite_CFLAGS = $(CGREEN_CFLAGS) $(CODE_COVERAGE_CFLAGS) $(GLFW_CFLAGS)
tests_topdax_suite_LDADD = topdax/libtopdax.a $(CGREEN_LIBS) $(CODE_COVERAGE_LIBS)

TESTS += tests/glfw_suite
check_PROGRAMS += tests/glfw_suite
tests_glfw_suite_SOURCES = tests/glfw_suite.c
tests_glfw_suite_CFLAGS = $(CGREEN_CFLAGS) $(CODE_COVERAGE_CFLAGS) $(GLFW_CFLAGS)
tests_glfw_suite_LDADD = topdax/libapplication.a $(CGREEN_LIBS) $(CODE_COVERAGE_LIBS)
tests_glfw_suite_LDFLAGS = -Wl,--wrap=argp_parse
endif
