ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include
@CODE_COVERAGE_RULES@
EXTRA_DIST = README.md LICENSE

noinst_LIBRARIES = libtopdax.a
libtopdax_a_SOURCES = src/topdax.c\
					  include/topdax/topdax.h
libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

bin_PROGRAMS = topdax
topdax_SOURCES = src/entry.c
topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_LDADD = libtopdax.a $(CODE_COVERAGE_LIBS)

if HAVE_CHECK
TESTS = topdax_test
check_PROGRAMS = topdax_test
topdax_test_SOURCES = tests/topdax_suite.c
topdax_test_CFLAGS = $(CHECK_CFLAGS) $(CODE_COVERAGE_CFLAGS)
topdax_test_LDADD = libtopdax.a $(CHECK_LIBS) $(CODE_COVERAGE_LIBS)
endif
