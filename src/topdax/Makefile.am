bin_PROGRAMS += topdax
topdax_SOURCES = src/GLFW/entry.c
topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_LDADD = librunloop.a libtopdax.a libtopdax_window.a liblogger.a libvkrenderer.a $(CODE_COVERAGE_LIBS)

noinst_LIBRARIES += libtopdax.a
libtopdax_a_SOURCES = src/topdax/topdax.c
libtopdax_a_SOURCES += src/topdax/topdax.h
libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += libtopdax_window.a
libtopdax_window_a_SOURCES = src/topdax/window.c
libtopdax_window_a_SOURCES += src/topdax/window.h
libtopdax_window_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += liblogger.a
liblogger_a_SOURCES = src/topdax/logger.c
liblogger_a_SOURCES += src/topdax/logger.h
liblogger_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += topdax_test
check_PROGRAMS += topdax_test
topdax_test_SOURCES = src/topdax/topdax_test.c
topdax_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_test_LDADD = libtopdax.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += topdax_window_test
check_PROGRAMS += topdax_window_test
topdax_window_test_SOURCES = src/topdax/window_test.c
topdax_window_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
topdax_window_test_LDADD = libtopdax_window.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += logger_test
check_PROGRAMS += logger_test
logger_test_SOURCES = src/topdax/logger_test.c
logger_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
logger_test_LDADD = liblogger.a -lcgreen $(CODE_COVERAGE_LIBS)
logger_test_LDFLAGS = -Wl,--wrap=fprintf
endif
