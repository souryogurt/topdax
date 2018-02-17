ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include

EXTRA_DIST = README.md LICENSE

bin_PROGRAMS = topdax
topdax_SOURCES = src/topdax.c\
				 include/topdax/topdax.h

if HAVE_CMOCKA
TESTS = topdax_test
check_PROGRAMS = topdax_test
topdax_test_SOURCES = tests/topdax_test.c
topdax_test_CFLAGS = $(CMOCKA_CFLAGS)
topdax_test_LDADD = $(CMOCKA_LIBS)
#topdax_test_LDFLAGS = -Wl,--wrap=symbol
endif
