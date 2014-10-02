#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libhpc], [0.1])
AC_CONFIG_SRCDIR([src/libhpc/libhpc.hh])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_AR

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stddef.h stdint.h stdlib.h string.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow select sqrt strdup])

# Features.
AC_ARG_ENABLE([instrument], [AS_HELP_STRING([--enable-instrument], [experimental instrumentation])])
AS_IF([test "x$enable_instrument" = "xno" -o "x$enable_instrument" = "x"], [
  AC_DEFINE([NINSTRUMENT], [1], [Instrumentation disabled])
])
AC_ARG_ENABLE([stacktrace], [AS_HELP_STRING([--enable-stacktrace], [generate assertion stacktraces])])
AS_IF([test "x$enable_stacktrace" = "xno" -o "x$enable_stacktrace" = "x"], [
  AC_DEFINE([NSTACKTRACE], [1], [Stacktrace disabled])
])
AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug], [debug mode])])
AS_IF([test "x$enable_debug" = "xno" -o "x$enable_debug" = "x"], [
  AC_DEFINE([NDEBUG], [1], [Debug disabled])
])
AS_IF([test "x$enable_debug" = "xyes"], [
  AC_SUBST([CXXFLAGS], ['-g -O0'])
])

AC_OUTPUT(makefile src/makefile)
