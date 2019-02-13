ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include

@CODE_COVERAGE_RULES@
EXTRA_DIST = LICENSE
dist_doc_DATA = README.md

bin_PROGRAMS =
noinst_LIBRARIES =
TESTS =
check_PROGRAMS =

include topdax/Makefile.am
include src/renderer/Makefile.am
include doc/Makefile.am
