TESTS =
check_PROGRAMS =

TESTS += check_config
check_PROGRAMS += check_config
check_config_SOURCES = ts_config.c $(top_builddir)/include/pushbullet.h
check_config_CFLAGS  = -I$(top_builddir)/include $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
check_config_LDFLAGS = $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
check_config_LDADD   = $(top_builddir)/lib/libpushbullet.la

TESTS += check_user
check_PROGRAMS += check_user
check_user_SOURCES = ts_user.c $(top_builddir)/include/pushbullet.h
check_user_CFLAGS  = -I$(top_builddir)/include $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
check_user_LDFLAGS = $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
check_user_LDADD   = $(top_builddir)/lib/libpushbullet.la

TESTS += check_device
check_PROGRAMS += check_device
check_device_SOURCES = ts_device.c $(top_builddir)/include/pushbullet.h
check_device_CFLAGS  = -I$(top_builddir)/include $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
check_device_LDFLAGS = $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
check_device_LDADD   = $(top_builddir)/lib/libpushbullet.la

TESTS += check_devices
check_PROGRAMS += check_devices
check_devices_SOURCES = ts_devices.c $(top_builddir)/include/pushbullet.h
check_devices_CFLAGS  = -I$(top_builddir)/include $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
check_devices_LDFLAGS = $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
check_devices_LDADD   = $(top_builddir)/lib/libpushbullet.la