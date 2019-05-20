AC_PREREQ([2.69])
AC_INIT([topdax],[0.4.0],[Egor Artemov <egor.artemov@gmail.com>],[topdax],[https://github.com/souryogurt/topdax])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([topdax/topdax.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror 1.11.1 foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE([enable])
# Checks for programs.
AC_PROG_CC
AM_PROG_AR
LT_INIT([win32-dll])

AC_CHECK_PROGS([ASCIIDOCTOR], [asciidoctor])
AS_IF([test -n "$ASCIIDOCTOR"],
      [AC_CHECK_PROG([GEM], [gem], [yes])]
)
AS_IF([test "x$GEM" = "xyes"],
      [AS_IF([gem which -q --silent "asciidoctor-diagram"],
	     [have_asciidoctor_diagram=yes],
	     [have_asciidoctor_diagram=yes])
      ],
      [have_asciidoctor_diagram=no]
)
AM_CONDITIONAL([HAVE_ASCIIDOCTOR], [test -n "$ASCIIDOCTOR"])
AM_CONDITIONAL([HAVE_ASCIIDOCTOR_DIAGRAM], [test "x$have_asciidoctor_diagram" = "xyes"])

AC_CHECK_HEADER([GLFW/glfw3.h], [], [
  AC_MSG_ERROR([GLFW/glfw3.h not found])
])
AC_CHECK_LIB([glfw], [glfwInit], [], [
  AC_MSG_ERROR([libglfw not found])
])

AC_CHECK_HEADER([vulkan/vulkan.h], [], [
  AC_MSG_ERROR([vulkan/vulkan.h not found])
])
AC_CHECK_LIB([vulkan], [vkCreateInstance], [], [
  AC_MSG_ERROR([libvulkan not found])
])

AX_SPLIT_VERSION
AC_DEFINE_UNQUOTED([VERSION_MAJOR], [${AX_MAJOR_VERSION}], [Major version number of package])
AC_DEFINE_UNQUOTED([VERSION_MINOR], [${AX_MINOR_VERSION}], [Minor version number of package])
AC_DEFINE_UNQUOTED([VERSION_PATCH], [${AX_POINT_VERSION}], [Patch version number of package])

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

AC_CONFIG_FILES([Makefile doc/attributes.adoc])
AC_OUTPUT
