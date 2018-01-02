TESTS = check_config
check_PROGRAMS = check_config
check_config_SOURCES = check_config.c $(top_builddir)/include/pushbullet.h
check_config_CFLAGS  = -I$(top_builddir)/include $(CHECK_CFLAGS) $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
check_config_LDFLAGS = $(CHECK_LIBS) $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
check_config_LDADD   = $(top_builddir)/lib/libpushbullet.la