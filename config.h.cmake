#cmakedefine HAVE_ASSERT_H          // Check for <assert.h>
#cmakedefine HAVE_FCNTL_H           // Check for <fcntl.h>
#cmakedefine HAVE_GETOPT_H          // Check for <getopt.h>
#cmakedefine HAVE_LIBGEN_H          // Check for <libgen.h>
#cmakedefine HAVE_MAGIC_H           // Check for <magic.h>
#cmakedefine HAVE_STDDEF_H          // Check for <stddef.h>
#cmakedefine HAVE_STDINT_H          // Check for <stdint.h>
#cmakedefine HAVE_STDIO_H           // Check for <stdio.h>
#cmakedefine HAVE_STDLIB_H          // Check for <stdlib.h>
#cmakedefine HAVE_STRING_H          // Check for <string.h>
#cmakedefine HAVE_SYS_STAT_H        // Check for <sys/stat.h>
#cmakedefine HAVE_SYS_TYPES_H       // Check for <sys/types.h>
#cmakedefine HAVE_UNISTD_H          // Check for <unistd.h>
#cmakedefine CURL_FOUND             // Check for libcurl
#cmakedefine EVENT_FOUND            // Check for libevent
#cmakedefine JANSSON_FOUND          // Check for jansson
#cmakedefine CONFIG_FOUND           // Check for libconfig

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif // HAVE_ASSERT_H

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif // HAVE_FCNTL_H

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif // HAVE_GETOPT_H

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif // HAVE_LIBGEN_H

#ifdef HAVE_MAGIC_H
#include <magic.h>
#endif // HAVE_MAGIC_H

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif // HAVE_STDDEF_H

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif // HAVE_STDINT_H

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif // HAVE_STDIO_H

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif // HAVE_STDLIB_H

#ifdef HAVE_STRING_H
#include <string.h>
#endif // HAVE_STRING_H

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif // HAVE_SYS_STAT_H

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef CURL_FOUND
#include <curl/curl.h>
#endif // CURL_FOUND

#ifdef EVENT_FOUND
#include <event2/event.h>
#endif // EVENT_FOUND

#ifdef JANSSON_FOUND
#include <jansson.h>
#endif // JANSSON_FOUND

#ifdef CONFIG_FOUND
#include <libconfig.h>
#endif // CONFIG_FOUND
