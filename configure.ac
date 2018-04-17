AC_PREREQ([2.69])
AC_INIT([topdax],[0.2.0],[Egor Artemov <egor.artemov@gmail.com>],[topdax],[https://github.com/souryogurt/topdax])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([topdax/topdax.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror 1.11.1 foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE([enable])
AM_PROG_AR
# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
PKG_PROG_PKG_CONFIG

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

PKG_CHECK_MODULES([GLFW], [glfw3 >= 3.2.1])

AC_DEFINE([TOPDAX_USE_PLATFORM_GLFW], [1], [Use GLFW for application and window])

AX_CODE_COVERAGE
AC_ARG_WITH([unit-tests],
    AS_HELP_STRING([--without-unit-tests], [Ignore presence of cgreen and disable unit tests]))

AS_IF([test "x$with_unit_tests" != "xno"],
      [PKG_CHECK_MODULES([CGREEN], [cgreen >= 1.0.0], [have_cgreen=yes], [have_cgreen=no])],
      [have_cgreen=no])

AS_IF([test "x$have_cgreen" != "xyes"],
      [AS_IF([test "x$with_unit_tests" = "xyes"],
             [AC_MSG_ERROR([unit tests requested but libcgreen not found])])
])

AM_CONDITIONAL([HAVE_CGREEN], [test "x$have_cgreen" != "xno"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
