lib_LTLIBRARIES = libpushbullet.la
libpushbullet_la_SOURCES = devices.c pushes.c requests.c user.c
libpushbullet_la_CFLAGS = -W -Wall -Wextra -Wno-unused-function -fmessage-length=0 -D_REENTRANT -I/usr/include/json-c
libpushbullet_la_LDFLAGS = -version-info 0:1:0


bin_PROGRAMS =  pushbullet
pushbullet_SOURCES = main.c devices.c pushes.c requests.c user.c
pushbullet_LDADD = -lcurl -ljson-c