ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include

EXTRA_DIST = README.md LICENSE

bin_PROGRAMS = topdax
topdax_SOURCES = src/topdax.c\
				 include/topdax/topdax.h

if HAVE_CHECK
TESTS = topdax_test
check_PROGRAMS = topdax_test
topdax_test_SOURCES = tests/topdax_suite.c
topdax_test_CFLAGS = $(CHECK_CFLAGS)
topdax_test_LDADD = $(CHECK_LIBS)
endif
