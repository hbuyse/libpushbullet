# Prelude
AC_PREREQ(2.52)

AC_INIT(pushbullet, 1.0.0, henri.buyse@gmail.com, pushbullet, https://github.com/hbuyse/libpushbullet)
AM_INIT_AUTOMAKE()

AC_PROG_MAKE_SET

# foreign ignores some GNU Coding Standards like
# * Makefile.am: installing './INSTALL'
# * Makefile.am: error: required file './NEWS' not found
# * Makefile.am: error: required file './README' not found
# * Makefile.am: error: required file './AUTHORS' not found
# * Makefile.am: error: required file './ChangeLog' not found

# enable silent build by default
AM_SILENT_RULES(no)


# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(curl/curl.h getopt.h json/json.h libgen.h stdio.h stdlib.h string.h sys/stat.h sys/types.h unistd.h)
AC_CONFIG_HEADERS(config.h)

# Checks for libraries.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_TYPE(ssize_t, int)

# Checks for library functions.
AC_FUNC_VPRINTF
AC_FUNC_MEMCMP
AC_CHECK_FUNCS(realloc strcasecmp strdup strerror snprintf vsnprintf vasprintf open vsyslog strncasecmp setlocale uselocale)

LT_INIT

# AX_COMPILE_CHECK_SIZEOF(int)
# AX_COMPILE_CHECK_SIZEOF(long)
# AX_COMPILE_CHECK_SIZEOF(size_t, [#include <stdint.h>])

AC_CONFIG_FILES(Makefile src/Makefile)

# Output files.
AC_OUTPUT