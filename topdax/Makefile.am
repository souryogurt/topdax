bin_PROGRAMS += topdax/topdax
topdax_topdax_SOURCES = topdax/entry.c
topdax_topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_topdax_LDADD = topdax/libtopdax.a topdax/libtopdax_window.a\
		      topdax/liblogger.a lib/libvkrenderer.a lib/libvkswapchain.a\
		      lib/libvkconfig.a lib/libvkconfig_families.a\
		      lib/libvkconfig_swapchain.a lib/libvkframe.a\
		      $(CODE_COVERAGE_LIBS)

noinst_LIBRARIES += topdax/libtopdax.a
topdax_libtopdax_a_SOURCES = topdax/topdax.c
topdax_libtopdax_a_SOURCES += topdax/topdax.h
topdax_libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += topdax/libtopdax_window.a
topdax_libtopdax_window_a_SOURCES = topdax/window.c
topdax_libtopdax_window_a_SOURCES += topdax/window.h
topdax_libtopdax_window_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += topdax/liblogger.a
topdax_liblogger_a_SOURCES = topdax/logger.c
topdax_liblogger_a_SOURCES += topdax/logger.h
topdax_liblogger_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += topdax/topdax_test
check_PROGRAMS += topdax/topdax_test
topdax_topdax_test_SOURCES = topdax/topdax_test.c
topdax_topdax_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_topdax_test_LDADD = topdax/libtopdax.a -lcgreen $(CODE_COVERAGE_LIBS)
topdax_topdax_test_LDFLAGS = -Wl,--wrap=argp_parse

TESTS += topdax/topdax_window_test
check_PROGRAMS += topdax/topdax_window_test
topdax_topdax_window_test_SOURCES = topdax/window_test.c
topdax_topdax_window_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_topdax_window_test_LDADD = topdax/libtopdax_window.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += topdax/logger_test
check_PROGRAMS += topdax/logger_test
topdax_logger_test_SOURCES = topdax/logger_test.c
topdax_logger_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_logger_test_LDADD = topdax/liblogger.a -lcgreen $(CODE_COVERAGE_LIBS)
topdax_logger_test_LDFLAGS = -Wl,--wrap=fprintf
endif
