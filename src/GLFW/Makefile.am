noinst_LIBRARIES += lib/librunloop.a
lib_librunloop_a_SOURCES = include/application/utils.h
lib_librunloop_a_SOURCES += include/application/runloop.h
lib_librunloop_a_SOURCES += include/GLFW/runloop.h
lib_librunloop_a_SOURCES += src/GLFW/runloop.c
lib_librunloop_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += bin/glfw_test
check_PROGRAMS += bin/glfw_test
bin_glfw_test_SOURCES = src/GLFW/glfw_test.c
bin_glfw_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
bin_glfw_test_LDADD = lib/librunloop.a -lcgreen $(CODE_COVERAGE_LIBS)
bin_glfw_test_LDFLAGS = -Wl,--wrap=argp_parse
endif
