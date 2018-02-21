AC_PREREQ([2.69])
AC_INIT([topdax],[0.1.0],[Egor Artemov <egor.artemov@gmail.com>],[topdax],[https://github.com/souryogurt/topdax])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([src/topdax.c])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE([1.11.1 foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE([enable])

# Checks for programs.
AC_PROG_CC
PKG_PROG_PKG_CONFIG

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

AX_CODE_COVERAGE
AC_ARG_WITH([unit-tests],
    AS_HELP_STRING([--without-unit-tests], [Ignore presence of check and disable unit tests]))

AS_IF([test "x$with_unit_tests" != "xno"],
      [PKG_CHECK_MODULES([CHECK], [check >= 0.11.0], [have_check=yes], [have_check=no])],
      [have_check=no])

AS_IF([test "x$have_check" != "xyes"],
      [AS_IF([test "x$with_unit_tests" = "xyes"],
             [AC_MSG_ERROR([unit tests requested but libcheck not found])])
])

AM_CONDITIONAL([HAVE_CHECK], [test "x$have_check" != "xno"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
