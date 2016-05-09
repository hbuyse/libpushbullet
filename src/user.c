/**
 * @file requests.c
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

void dump_user_info(PB_user_t user)
{
    fprintf(stdout, "\e[1mtoken_key =\e[0m %s\n", user.token_key);
    fprintf(stdout, "\e[1mactive =\e[0m %u\n", user.active);
    fprintf(stdout, "\e[1memail =\e[0m %s\n", user.email);
    fprintf(stdout, "\e[1memail_normalized =\e[0m %s\n", user.email_normalized);
    fprintf(stdout, "\e[1miden =\e[0m %s\n", user.iden);
    fprintf(stdout, "\e[1mimage_url =\e[0m %s\n", user.image_url);
    fprintf(stdout, "\e[1mname =\e[0m %s\n", user.name);
}



unsigned char pb_get_user_info(PB_user_t *user,
                          char      *token_key
                          )
{
    char                *result     = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    unsigned char       res         = 0;
    json_object         *json_obj   = NULL;


    // Copy the token
    user->token_key = token_key;


    // Access the API using the token
    res = pb_get(result, API_URL_ME, user->token_key);

    if ( res != HTTP_OK )
    {
        return (res);
    }

    #ifdef __DEBUG__
    fprintf(stdout, "\e[1m[%s]\e[0m %s\n", __func__, result);
    #endif

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