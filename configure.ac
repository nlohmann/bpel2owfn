AC_INIT([GNU BPEL2oWFN],[2.5-unreleased],[bpel2owfn@service-technology.org],[bpel2owfn],[http://service-technology.org/bpel2owfn])

AC_MSG_NOTICE([ ____  ____  _____ _     ____    __        _______ _   _ ])
AC_MSG_NOTICE([| __ )|  _ \| ____| |   |___ \ __\ \      / /  ___| \ | |])
AC_MSG_NOTICE([|  _ \| |_) |  _| | |     __) / _ \ \ /\ / /| |_  |  \| |])
AC_MSG_NOTICE([| |_) |  __/| |___| |___ / __/ (_) \ V  V / |  _| | |\  |])
AC_MSG_NOTICE([|____/|_|   |_____|_____|_____\___/ \_/\_/  |_|   |_| \_|])
AC_MSG_NOTICE([translating WS-BPEL processes into Petri net models])
AC_MSG_NOTICE([http://service-technology.org/bpel2owfn])
AC_MSG_NOTICE([http://www.gnu.org/software/bpel2owfn])
AC_MSG_NOTICE([version ${PACKAGE_VERSION}])
AC_MSG_NOTICE()

AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([build-aux])


#############################################################################
# check for prerequisites, programms, headers, and libraries
#############################################################################

# use the service-technology.org macros
AC_ST_BASIC
AC_ST_AUTOTEST

# check for generic programs
AC_PROG_CXX
AC_PROG_YACC
AC_PROG_LEX
AC_PROG_SED
AM_PROG_CC_C_O

# check for additional programs
AM_MISSING_PROG(HELP2MAN, help2man)
AM_MISSING_PROG(GENGETOPT, gengetopt)
AM_MISSING_PROG(KC, kc++)
AC_PATH_PROG(DOT, [dot], not found)
if test "$DOT" = "not found"; then
 AC_DEFINE(HAVE_DOT, 0, [Graphviz dot can not be used on this system.])
else
 AC_DEFINE(HAVE_DOT, 1, [Graphviz dot can be used on this system.])
fi

# store the SVN revision number
AC_DEFINE_UNQUOTED(VERSION_SVN, "`svnversion -n 2> /dev/null`", [The SVN revision.])

# check for header to color console output
AC_CHECK_HEADERS([windows.h])

# look up canonical build name and write it to config.h
AC_CANONICAL_BUILD
AC_DEFINE_UNQUOTED([BUILDSYSTEM], ["$build"], [The platform.])

# store the SVN revision number
AC_SUBST(VERSION_SVN, "`svnversion -n 2> /dev/null`")
AC_DEFINE_UNQUOTED(VERSION_SVN, "${VERSION_SVN}", [The SVN revision.])


#############################################################################
# create output files
#############################################################################

# Set files to generate.
AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile doc/Doxyfile man/Makefile tests/Makefile tests/package.m4 gui/static_release.sh])
AC_CONFIG_FILES([tests/cover.sh], [chmod +x tests/cover.sh])
AC_OUTPUT

# fix permissions of generated shell scripts
chmod a+x gui/static_release.sh
