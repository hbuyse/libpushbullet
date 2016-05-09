/**
 * @file devices.c
 * @date 08/05/2016
 */

#include <stdio.h>          // fprintf, stdout

#include <requests.h>          // pb_get, API_URL_DEVICES
#include <user.h>          // PB_user_t
#include <json/json.h>          // json_object
#include <http_code.h>          // HTTP_OK


#define     MAX_SIZE_BUF 4096


unsigned short pb_get_devices(PB_user_t user)
{
    char                *result = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    unsigned char       res     = 0;
    json_object         *json_obj __attribute__( (unused) ) = NULL;


    res = pb_get(result, API_URL_DEVICES, user.token_key);

    #ifdef __DEBUG__
    fprintf( (res == HTTP_OK) ? stdout : stderr, "\e[1m[%s]\e[0m %s\n", __func__, result);
    #endif

    if ( result )
    {
        free(result);
    }

    return (res);
}