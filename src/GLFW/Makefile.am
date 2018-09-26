noinst_LIBRARIES += librunloop.a
librunloop_a_SOURCES = include/application/utils.h
librunloop_a_SOURCES += include/application/runloop.h
librunloop_a_SOURCES += include/GLFW/runloop.h
librunloop_a_SOURCES += src/GLFW/runloop.c
librunloop_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)

if HAVE_CGREEN
TESTS += glfw_test
check_PROGRAMS += glfw_test
glfw_test_SOURCES = src/GLFW/glfw_test.c
glfw_test_CFLAGS = $(CODE_COVERAGE_CFLAGS)
glfw_test_LDADD = librunloop.a -lcgreen $(CODE_COVERAGE_LIBS)
glfw_test_LDFLAGS = -Wl,--wrap=argp_parse
endif
