# serial 1
AC_DEFUN([AC_ST_BASIC],[

# configure Automake
AM_INIT_AUTOMAKE(gnits)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# check for standard programs needed to compile
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_LEX
AC_PROG_YACC

# store the SVN revision number
AC_SUBST(VERSION_SVN, "`svnversion -n 2> /dev/null`")
AC_DEFINE_UNQUOTED([VERSION_SVN], ["${VERSION_SVN}"], [The SVN revision.])

# look up canonical build name and write it to config.h
AC_CANONICAL_BUILD
AC_DEFINE_UNQUOTED([CONFIG_BUILDSYSTEM], ["${build}"], [The platform.])

# check for basic tools (also important for GNU Autotest)
AC_PROG_SED
AC_PROG_GREP
AC_PROG_AWK

# export tools needed to determine memory consumption
AC_DEFINE_UNQUOTED(TOOL_AWK, "\"${AWK}\"", [awk])
AC_DEFINE_UNQUOTED(TOOL_GREP, "\"${GREP}\"", [grep])

# check for additional programs needed to compile
AM_MISSING_PROG(GENGETOPT, gengetopt)
AM_MISSING_PROG(HELP2MAN, help2man)

# allow the configure script to control assertions (just include config.h)
AC_HEADER_ASSERT
AH_BOTTOM([#ifdef __cplusplus
#include <cassert>
#else
#include <assert.h>
#endif])

])