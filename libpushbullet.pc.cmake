prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=${exec_prefix}/@CMAKE_INSTALL_LIBDIR@
includedir=${prefix}/@CMAKE_INSTALL_INCLUDEDIR@

Name: @PROJECT_NAME@
Description: @PROJECT_DESCRIPTION@
Version: @PROJECT_VERSION@
Requires:
Conflicts:
Libs: -L${libdir} -lpushbullet
Cflags: -I${includedir}
Requires: glib-2.0 >= 2.40.2 json-glib-1.0 >= 0.16.2 libcurl >= 7.35.0
