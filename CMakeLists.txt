cmake_minimum_required(VERSION 3.0)
project(libpushbullet VERSION 0.1.0 LANGUAGES C)

set(PROJECT_DESCRIPTION "Pushbullet library")

# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE off)

# check_include_files
include(CheckIncludeFiles)

# CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR, CMAKE_INSTALL_DATAROOTDIR
include(GNUInstallDirs)

# check_symbol_exists
include(CheckSymbolExists)

# Check if functions exists
include(CheckFunctionExists)

check_include_files(assert.h HAVE_ASSERT_H)
check_include_files(fcntl.h HAVE_FCNTL_H)
check_include_files(getopt.h HAVE_GETOPT_H)
check_include_files(libgen.h HAVE_LIBGEN_H)
check_include_files(magic.h HAVE_MAGIC_H)
check_include_files(stddef.h HAVE_STDDEF_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdio.h HAVE_STDIO_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)

# Pkg Config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl>=7.58.0)
pkg_check_modules(JANSSON REQUIRED jansson>=2.11)
pkg_check_modules(CONFIG REQUIRED libconfig>=1.4.9)
pkg_check_modules(EVENT libevent>=2.0.5)

# Check if _GNU_SOURCE is available.
if (NOT DEFINED _GNU_SOURCE)
  check_symbol_exists(__GNU_LIBRARY__ "features.h" _GNU_SOURCE)

  if (NOT _GNU_SOURCE)
    unset(_GNU_SOURCE CACHE)
    check_symbol_exists(_GNU_SOURCE "features.h" _GNU_SOURCE)
  endif(NOT _GNU_SOURCE)

  check_function_exists(vasprintf HAVE_VASPRINTF_FN)
  check_function_exists(asprintf HAVE_ASPRINTF_FN)
endif(NOT DEFINED _GNU_SOURCE)

# Library
include(lib/CMakeLists.txt)

# daemon
if (EVENT_FOUND)
  include(daemon/CMakeLists.txt)
endif (EVENT_FOUND)


# include(examples/CMakeLists.txt)

# FILE CONFIGURATION
configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
configure_file(${CMAKE_SOURCE_DIR}/libpushbullet.pc.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/libpushbullet.pc)
