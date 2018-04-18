AC_PREREQ([2.69])
AC_INIT([topdax],[0.3.0],[Egor Artemov <egor.artemov@gmail.com>],[topdax],[https://github.com/souryogurt/topdax])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/topdax.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror 1.11.1 foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE([enable])
AM_PROG_AR
# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

AC_CHECK_HEADER([GLFW/glfw3.h], [], [
  AC_MSG_ERROR([GLFW/glfw3.h not found])
])
AC_CHECK_LIB([glfw], [glfwInit], [], [
  AC_MSG_ERROR([libglfw not found])
])
AC_DEFINE([TOPDAX_USE_PLATFORM_GLFW], [1], [Use GLFW for application and window])

AX_CODE_COVERAGE
AC_ARG_WITH([unit-tests],
    AS_HELP_STRING([--without-unit-tests], [Ignore presence of cgreen and disable unit tests]))

AS_IF([test "x$with_unit_tests" != "xno"],
      [AC_CHECK_HEADER([cgreen/cgreen.h],
       [AC_CHECK_LIB([cgreen], [run_test_suite], [have_cgreen=yes], [have_cgreen=no])],
       [have_cgreen=no])],
      [have_cgreen=no])

AS_IF([test "x$have_cgreen" != "xyes"],
      [AS_IF([test "x$with_unit_tests" = "xyes"],
             [AC_MSG_ERROR([unit tests requested but cgreen/cgreen.h not found])])
])
AM_CONDITIONAL([HAVE_CGREEN], [test "x$have_cgreen" != "xno"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
