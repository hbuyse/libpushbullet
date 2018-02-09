AM_CFLAGS = -W -Wall -Wextra -fmessage-length=0 -D_REENTRANT -I$(top_builddir)/include/

if ENABLE_DEBUG
AM_CFLAGS  += -D__DEBUG__ -ggdb3 -O0
endif

if ENABLE_TRACES
AM_CFLAGS  += -D__TRACES__
endif

lib_LTLIBRARIES          = libpushbullet.la
libpushbullet_la_SOURCES = pb_config.c pb_requests.c pb_user.c pb_device.c pb_devices.c pb_pushes.c pb_session.c
libpushbullet_la_CFLAGS  = $(AM_CFLAGS) $(JSON_GLIB_CFLAGS) $(LIBCURL_CFLAGS)
libpushbullet_la_LDFLAGS = -version-info 0:1:0
libpushbullet_la_LDFLAGS += $(JSON_GLIB_LIBS) $(LIBCURL_LIBS)