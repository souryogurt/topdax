bin_PROGRAMS += bin/topdax
bin_topdax_SOURCES = src/GLFW/entry.c
bin_topdax_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_topdax_LDADD = lib/librunloop.a lib/libtopdax.a lib/libtopdax_window.a\
		   lib/liblogger.a lib/libvkrenderer.a lib/libvkconfig.a\
		   lib/libvkconfig_families.a lib/libvkconfig_swapchain.a\
		   $(CODE_COVERAGE_LIBS)

noinst_LIBRARIES += lib/libtopdax.a
lib_libtopdax_a_SOURCES = src/topdax/topdax.c
lib_libtopdax_a_SOURCES += src/topdax/topdax.h
lib_libtopdax_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += lib/libtopdax_window.a
lib_libtopdax_window_a_SOURCES = src/topdax/window.c
lib_libtopdax_window_a_SOURCES += src/topdax/window.h
lib_libtopdax_window_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

noinst_LIBRARIES += lib/liblogger.a
lib_liblogger_a_SOURCES = src/topdax/logger.c
lib_liblogger_a_SOURCES += src/topdax/logger.h
lib_liblogger_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += bin/topdax_test
check_PROGRAMS += bin/topdax_test
bin_topdax_test_SOURCES = src/topdax/topdax_test.c
bin_topdax_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_topdax_test_LDADD = lib/libtopdax.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += bin/topdax_window_test
check_PROGRAMS += bin/topdax_window_test
bin_topdax_window_test_SOURCES = src/topdax/window_test.c
bin_topdax_window_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_topdax_window_test_LDADD = lib/libtopdax_window.a -lcgreen $(CODE_COVERAGE_LIBS)

TESTS += bin/logger_test
check_PROGRAMS += bin/logger_test
bin_logger_test_SOURCES = src/topdax/logger_test.c
bin_logger_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_logger_test_LDADD = lib/liblogger.a -lcgreen $(CODE_COVERAGE_LIBS)
bin_logger_test_LDFLAGS = -Wl,--wrap=fprintf
endif
