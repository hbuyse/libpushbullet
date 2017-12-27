lib_LTLIBRARIES = libpushbullet.la
libpushbullet_la_SOURCES = devices.c pushes.c requests.c user.c
libpushbullet_la_CFLAGS = -W -Wall -Wextra -Wno-unused-function -fmessage-length=0 -D_REENTRANT $(curl_CFLAGS) $(jsonc_CFLAGS)
libpushbullet_la_LDFLAGS = -version-info 0:1:0 $(curl_LIBS) $(jsonc_LIBS)


bin_PROGRAMS =  pushbullet
pushbullet_SOURCES = main.c
pushbullet_CFLAGS = -W -Wall -Wextra -Wno-unused-function -fmessage-length=0 -D_REENTRANT $(curl_CFLAGS) $(jsonc_CFLAGS)
pushbullet_LDFLAGS = $(curl_LIBS) $(jsonc_LIBS)
pushbullet_LDADD = libpushbullet.la