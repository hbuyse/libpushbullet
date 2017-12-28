AM_CFLAGS = -W -Wall -Wextra -ggdb -fmessage-length=0 -D_REENTRANT


lib_LTLIBRARIES          = libpushbullet.la
libpushbullet_la_SOURCES = devices.c pushes.c requests.c user.c
libpushbullet_la_CFLAGS  = $(AM_CFLAGS) $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
libpushbullet_la_LDFLAGS = -version-info 0:1:0
libpushbullet_la_LDFLAGS += $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)


bin_PROGRAMS       = pushbullet
pushbullet_SOURCES = main.c
pushbullet_CFLAGS  = $(AM_CFLAGS) $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
pushbullet_LDFLAGS = $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
pushbullet_LDADD   = libpushbullet.la