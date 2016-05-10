/**
 * @file user.c
 * @date 08/05/2016
 */

#include <stdio.h>          // fprintf, stderr
#include <stdlib.h>          // calloc, free, abort
#include <string.h>          // strcmp

#include <requests.h>          // pb_get
#include <user.h>          // PB_user_t
#include <json/json.h>          // json_object, json_tokener_parse, json_object_get_string, json_object_object_foreach,
                                // json_object_get_int
#include <http_code.h>          // HTTP_OK

#define     MAX_SIZE_BUF 4096

static void _dump_user_info(PB_user_t user)
{
    // fprintf(stdout, "\e[1mtoken_key =\e[0m %s\n", user.token_key);
    fprintf(stdout, "\e[1m[%s]\e[0m %s - %s\n", __func__, user.name, user.email);
    fprintf(stdout, "\e[1m[%s]\e[0m\tactive : %u\n", __func__, user.active);
    fprintf(stdout, "\e[1m[%s]\e[0m\tcreated : %f\n", __func__, user.created);
    fprintf(stdout, "\e[1m[%s]\e[0m\tmodified : %f\n", __func__, user.modified);
    fprintf(stdout, "\e[1m[%s]\e[0m\temail_normalized : %s\n", __func__, user.email_normalized);
    fprintf(stdout, "\e[1m[%s]\e[0m\tiden : %s\n", __func__, user.iden);
    fprintf(stdout, "\e[1m[%s]\e[0m\timage_url : %s\n", __func__, user.image_url);
    fprintf(stdout, "\e[1m[%s]\e[0m\tmax_upload_size : %d\n", __func__, user.max_upload_size);
}



unsigned char pb_get_user_info(PB_user_t    *user,
                               char         *token_key
                               )
{
    char                *result     = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    unsigned char       res         = 0;
    json_object         *json_obj   = NULL;


    // Copy the token
    user->token_key = token_key;


    // Access the API using the token
    res = pb_get(result, API_URL_ME, user->token_key);

    #ifdef __DEBUG__
    fprintf( (res == HTTP_OK) ? stdout : stderr, "\e[1m[%s]\e[0m %s\n", __func__, result);
    #endif

    if ( res != HTTP_OK )
    {
        return (res);
    }

    json_obj = json_tokener_parse(result);
    json_object_object_foreach(json_obj, key, val)
    {
        if ( strcmp(key, "active") == 0 )
        {
            user->active = json_object_get_int(val);
        }
        else if ( strcmp(key, "email") == 0 )
        {
            user->email = json_object_get_string(val);
        }
        else if ( strcmp(key, "email_normalized") == 0 )
        {
            user->email_normalized = json_object_get_string(val);
        }
        else if ( strcmp(key, "iden") == 0 )
        {
            user->iden = json_object_get_string(val);
        }
        else if ( strcmp(key, "image_url") == 0 )
        {
            user->image_url = json_object_get_string(val);
        }
        else if ( strcmp(key, "name") == 0 )
        {
            user->name = json_object_get_string(val);
        }
    }

    if ( result )
    {
        free(result);
    }

    return (res);
}