TESTS = check_config
check_PROGRAMS = check_pb
check_pb_SOURCES = main.c ts_config.c $(top_builddir)/include/pushbullet.h
check_pb_CFLAGS  = -I$(top_builddir)/include $(CHECK_CFLAGS) $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
check_pb_LDFLAGS = $(CHECK_LIBS) $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
check_pb_LDADD   = $(top_builddir)/lib/libpushbullet.la