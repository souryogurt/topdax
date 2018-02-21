ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include
@CODE_COVERAGE_RULES@
EXTRA_DIST = README.md LICENSE

bin_PROGRAMS = topdax
topdax_SOURCES = src/topdax.c\
				 include/topdax/topdax.h
topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_LDADD = $(CODE_COVERAGE_LIBS)

if HAVE_CHECK
TESTS = topdax_test
check_PROGRAMS = topdax_test
topdax_test_SOURCES = tests/topdax_suite.c
topdax_test_CFLAGS = $(CHECK_CFLAGS) $(CODE_COVERAGE_CFLAGS)
topdax_test_LDADD = $(CHECK_LIBS) $(CODE_COVERAGE_LIBS)
endif
