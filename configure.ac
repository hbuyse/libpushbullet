# Prelude
AC_PREREQ(2.52)

AC_INIT([pushbullet], [1.0.0], [henri.buyse@gmail.com], [pushbullet], [https://github.com/hbuyse/libpushbullet])
AM_INIT_AUTOMAKE([foreign])

PACKAGE_DESCRIPTION="Pushbullet library"
AC_SUBST(PACKAGE_DESCRIPTION)

AC_PROG_MAKE_SET

# enable silent build by default
AM_SILENT_RULES([yes])

# Checks for programs.
# AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
# AC_PROG_CC_STDC
# AC_PROG_CXX
AM_PROG_AR
AC_PROG_LN_S
AC_PROG_MKDIR_P
AC_SYS_LARGEFILE
PKG_PROG_PKG_CONFIG


#AC_REVISION([m4_esyscmd_s([git describe --always --dirty])])
AC_REVISION([m4_esyscmd_s([git describe --always])])
# saves revision to configure

GIT_VERSION=`git describe --always --dirty`
if test x"$GIT_VERSION" = x; then
   AC_DEFINE(GIT_VERSION, ["unknown"], [Git revision])
   AC_SUBST(GIT_VERSION)
else
   AC_DEFINE_UNQUOTED(GIT_VERSION, "$GIT_VERSION", [Git revision])
   AC_SUBST(GIT_VERSION)
fi


# Check pkg-config
PKG_CHECK_MODULES([JSON_GLIB], [json-glib-1.0 >= 0.16.2])
PKG_CHECK_MODULES([LIBCURL], [libcurl >= 7.35.0])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([getopt.h libgen.h stdio.h stdlib.h string.h sys/stat.h sys/types.h unistd.h])
AC_CONFIG_HEADERS([config.h])

# Checks for libraries.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_TYPE(ssize_t, int)

# Checks for library functions.
AC_FUNC_VPRINTF
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([realloc strcasecmp strdup strerror snprintf vsnprintf vasprintf open vsyslog strncasecmp setlocale uselocale])

LT_INIT

AC_CONFIG_FILES([Makefile src/Makefile lib/Makefile tests/Makefile libpushbullet.pc])

# --enable-doc
AC_ARG_ENABLE([doc],
  AS_HELP_STRING([--enable-doc], [Build documentation (needs doxygen)]),
  [enable_doc=$enableval],[enable_doc=no])
AM_CONDITIONAL([ENABLE_DOC], [test "x$enable_doc" = "xyes"])
AM_COND_IF([ENABLE_DOC], [AC_CONFIG_FILES([Doxyfile])])
if test "${enable_doc}" = "yes"; then
  AC_CHECK_PROG([DOXYGEN], [doxygen], [yes], [no])
  AC_CHECK_PROG([DOT], [dot], [yes], [no])
fi

AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
  [enable_debug=$enableval],[enable_debug=no])
AM_CONDITIONAL([ENABLE_DEBUG], [test x"$enable_debug" = x"yes"])

AC_ARG_ENABLE([traces],
  AS_HELP_STRING([--enable-traces], [enable traces, default: no]),
  [enable_traces=$enableval],[enable_traces=no])
AM_CONDITIONAL([ENABLE_TRACES], [test x"$enable_traces" = x"yes"])
if test "${enable_traces}" = "yes"; then
  AC_DEFINE([TRACES], [1], [Define to 1 to enable the traces])
fi

AC_CHECK_LIB(magic, [magic_open], [], [
  AC_MSG_ERROR([unable to find the magic_open() function])
])

# Output files.
AC_OUTPUT

# Display infos
AC_MSG_RESULT([
    $PACKAGE $VERSION - ${GIT_VERSION}

        CC:                     ${CC}
        CFLAGS:                 ${CFLAGS}
        CPPFLAGS:               ${CPPFLAGS}
        LDFLAGS:                ${LDFLAGS}
        LIBS:                   ${LIBS}
        DEBUG:                  ${enable_debug}
        TRACES:                 ${enable_traces}

        prefix:                 ${prefix}
        sysconfdir:             ${sysconfdir}
        libdir:                 ${libdir}
        includedir:             ${includedir}
        pkgconfigdir:           ${pkgconfigdir}

        Build documentation:    ${enable_doc}
        Doxygen:                ${DOXYGEN}
        Dot:                    ${DOT}
])
