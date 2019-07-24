ACLOCAL_AMFLAGS = -I m4

@CODE_COVERAGE_RULES@
EXTRA_DIST = LICENSE
dist_doc_DATA = README.md

bin_PROGRAMS =
noinst_LTLIBRARIES =
TESTS =
check_LTLIBRARIES =
check_PROGRAMS =

AM_CFLAGS = $(CODE_COVERAGE_CFLAGS)
AM_CPPFLAGS = $(CODE_COVERAGE_CFLAGS)

include topdax/Makefile.am
include renderer/Makefile.am
include doc/Makefile.am
