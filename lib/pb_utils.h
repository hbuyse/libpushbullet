/**
 * @file pb_utils.h
 * @author hbuyse
 * @date 17/06/2016
 *
 * @brief  Common utilitary macro
 */


#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdio.h>          // fprintf, stderr, stdout
#include <stdlib.h>          // free

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief      Print an information on the standard output
 *
 * @param      format  The format
 * @param      ...     The list of the arguments
 */
#define iprintf(format, ...)    fprintf(stdout, "\e[1m[%s:%d %s]\e[0m " format, __FILE__, __LINE__, __func__, ## __VA_ARGS__)


/**
 * @brief      Print an error on the standard error output
 *
 * @param      format  The format
 * @param      ...     The list of the arguments
 */
#define eprintf(format, ...)    fprintf(stderr, "\e[1;31m[%s:%d %s]\e[0m " format, __FILE__, __LINE__, __func__, ## __VA_ARGS__)


/**
 * @brief      Print a warning (caution) on the standard output
 *
 * @param      format  The format
 * @param      ...     The list of the arguments
 */
#define cprintf(format, ...)    fprintf(stdout, "\e[1;33m[%s:%d %s]\e[0m " format, __FILE__, __LINE__, __func__, ## __VA_ARGS__)


/**
 * @brief      Print on the standard output
 *
 * @param      format  The format
 * @param      ...     The list of the arguments
 */
#define gprintf(format, ...)    fprintf(stdout, "\e[1;32m[%s:%d %s]\e[0m " format, __FILE__, __LINE__, __func__, ## __VA_ARGS__)


#define print_json_node_to_stream(method, jn) \
    do {    \
        JsonGenerator* gen = json_generator_new();  \
        json_generator_set_root(gen, jn);   \
        gchar* buf = json_generator_to_data(gen, NULL); \
        if (buf) {  \
            method("%s\n", buf);    \
            free(buf);  \
        }   \
    } while (0)



/**
 * @brief      Free a pointer if it exists
 *
 * @param      ptr   The pointer
 */
#define     pb_free(ptr)                             \
    if ( ptr ) {free( (void *) ptr); ptr = NULL; }

#ifdef     __cplusplus
}
#endif

#endif          // __LOGGING_H__