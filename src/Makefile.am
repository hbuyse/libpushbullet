AM_CFLAGS = -W -Wall -Wextra -ggdb -fmessage-length=0 -D_REENTRANT -I$(top_builddir)/include/

if ENABLE_DEBUG
AM_CFLAGS  += -D__DEBUG__
endif

bin_PROGRAMS       = pushbullet
pushbullet_SOURCES = main.c
pushbullet_CFLAGS  = $(AM_CFLAGS) $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
pushbullet_LDFLAGS = $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)
pushbullet_LDADD   = $(top_builddir)/lib/libpushbullet.la