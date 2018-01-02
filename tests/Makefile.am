TESTS = check_config check_user

check_PROGRAMS = check_config check_user

check_config_SOURCES = ts_config.c $(top_builddir)/include/pushbullet.h
check_config_CFLAGS  = -I$(top_builddir)/include $(CHECK_CFLAGS) $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
check_config_LDFLAGS = $(CHECK_LIBS) $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
check_config_LDADD   = $(top_builddir)/lib/libpushbullet.la

check_user_SOURCES = ts_user.c $(top_builddir)/include/pushbullet.h
check_user_CFLAGS  = -I$(top_builddir)/include $(CHECK_CFLAGS) $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
check_user_LDFLAGS = $(CHECK_LIBS) $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
check_user_LDADD   = $(top_builddir)/lib/libpushbullet.la