ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include

EXTRA_DIST = README.md LICENSE

bin_PROGRAMS = topdax
topdax_SOURCES = src/topdax.c\
				 include/topdax/topdax.h

if HAVE_CHECK
TESTS = topdax_unittests
check_PROGRAMS = topdax_unittests
topdax_unittests_SOURCES = tests/runner.c\
						   tests/topdax_suite.c
topdax_unittests_CFLAGS = $(CHECK_CFLAGS)
topdax_unittests_LDADD = $(CHECK_LIBS)
endif
