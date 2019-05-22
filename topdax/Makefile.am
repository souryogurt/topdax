bin_PROGRAMS += topdax/topdax
topdax_topdax_SOURCES = topdax/entry.c
topdax_topdax_LDADD = topdax/libtopdax.la\
		      topdax/liblogger.la\
		      renderer/libvkrenderer.la\
		      renderer/libvkswapchain.la\
		      renderer/libvkconfig.la\
		      renderer/libvkconfig_families.la\
		      renderer/libvkconfig_swapchain.la\
		      renderer/libvkframe.la\
		      $(CODE_COVERAGE_LIBS)

noinst_LTLIBRARIES += topdax/libtopdax.la
topdax_libtopdax_la_SOURCES = topdax/topdax.c\
			      topdax/topdax.h

noinst_LTLIBRARIES += topdax/liblogger.la
topdax_liblogger_la_SOURCES = topdax/logger.c\
			      topdax/logger.h

if HAVE_CGREEN
TESTS += topdax/topdax_test
check_PROGRAMS += topdax/topdax_test
topdax_topdax_test_SOURCES = topdax/topdax_test.c
topdax_topdax_test_LDADD = topdax/libtopdax.la -lcgreen $(CODE_COVERAGE_LIBS)
topdax_topdax_test_LDFLAGS = -Wl,--wrap=argp_parse

TESTS += topdax/logger_test
check_PROGRAMS += topdax/logger_test
topdax_logger_test_SOURCES = topdax/logger_test.c
topdax_logger_test_LDADD = topdax/liblogger.la -lcgreen $(CODE_COVERAGE_LIBS)
topdax_logger_test_LDFLAGS = -Wl,--wrap=fprintf
endif
