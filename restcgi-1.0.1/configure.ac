#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
# RELEASING: update this software version number to match the svn release tag
# ALSO UPDATE LIBRARY VERSION NUMBER IN: src/Makefile.am
AC_INIT([restcgi], [1.0.1])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
LT_INIT
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/rest.h])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([uripp], [_ZN5uripp4pathC1Ev], [], [AC_MSG_ERROR([missing uripp library (see http://sourceforge.net/projects/uripp)])])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h],[],[AC_MSG_ERROR([missing std headers])])
#doesn't ever find it: AC_CHECK_HEADER([boost/shared_ptr.hpp],[],[AC_MSG_ERROR([missing boost headers])])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset strcasecmp strchr],[],[AC_MSG_ERROR([missing std functions])])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    test/Makefile
])
AC_OUTPUT
