/**
 * @file user.c
 * @author hbuyse
 * @date 08/05/2016
 */

#include <stdio.h>          // fprintf, stderr
#include <stdlib.h>          // calloc, free, abort
#include <string.h>          // strcmp
#include <json/json.h>          // json_object, json_tokener_parse, json_object_get_string, json_object_object_foreach,
                                // json_object_get_int

#include <urls.h>           // API_URL_ME
#include <requests.h>          // pb_get
#include <pb_structures.h>          // PB_user_t
#include <http_code.h>          // HTTP_OK
#include <devices.h>          // pb_free_devices


/**
 * \brief Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000


/**
 * \brief      Macro to associate a key in a structure
 *
 * \param      type  The type
 * \param      var   The pointer we fill
 * \param      k     The JSON key
 */
#define     JSON_ASSOCIATE(type, var, k)          \
    if ( strcmp(key, # k) == 0 ) {var->k = json_object_get_ ## type(val); }


/**
 * \brief      Free a pointer if it exists
 *
 * \param      ptr   The pointer
 */
#define     FREE(ptr)                             \
    if ( ptr ) {free( (void *) ptr); ptr = NULL; }

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



unsigned short pb_get_user_info(PB_user_t   *user,
                                char        *token_key
                                )
{
    char                *result     = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    unsigned short      res         = 0;
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
        JSON_ASSOCIATE(int, user, active)
        JSON_ASSOCIATE(double, user, created)
        JSON_ASSOCIATE(double, user, modified)
        JSON_ASSOCIATE(string, user, email)
        JSON_ASSOCIATE(string, user, email_normalized)
        JSON_ASSOCIATE(string, user, iden)
        JSON_ASSOCIATE(string, user, image_url)
        JSON_ASSOCIATE(string, user, name)
        JSON_ASSOCIATE(int, user, max_upload_size)
    }

    #ifdef __DEBUG__
    _dump_user_info(*user);
    #endif

    if ( result )
    {
        free(result);
    }

    return (res);
}



void pb_free_user(PB_user_t *user)
{
    if ( user )
    {
        /* Do not remove user->token_key because it causes a munmap_chunk since the token_key is given as an argument of
         * the program.
         * The free() function frees the memory space pointed to by ptr, which must have been returned by a previous
         * call to malloc(), calloc() or realloc(). Otherwise, or if free(ptr) has already been called before, undefined
         * behavior occurs. If ptr is NULL, no operation is performed.
         * FREE(user->token_key);
         */
        user->token_key         = NULL;
        user->active            = 0;
        user->created           = 0;
        user->modified          = 0;
        user->max_upload_size   = 0;

        FREE(user->email);
        FREE(user->email_normalized);
        FREE(user->iden);
        FREE(user->image_url);
        FREE(user->name);

        pb_free_devices(user);
    }
}