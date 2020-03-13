/**
 * @file pb_user.c
 * @author hbuyse
 * @date 08/05/2016
 */

#include <stdio.h>          // fprintf, stderr
#include <stdlib.h>          // calloc, free, abort
#include <string.h>          // strcmp
#include <sys/types.h>          // struct stat, stat, S_ISREG
#include <sys/stat.h>          // struct stat, stat, S_ISREG
#include <unistd.h>          // struct stat, stat, S_ISREG

#include <json-glib/json-glib.h>          // json_object, json_tokener_parse, json_object_get_string, json_object_object_foreach,
                                // json_object_get_int

#include "pb_user_priv.h"        // pb_user_t, MAX_SIZE_BUF, JSON_ASSOCIATE_BOOL, JSON_ASSOCIATE_STR, JSON_ASSOCIATE_INT, JSON_ASSOCIATE_DOUBLE
#include "pb_user_prot.h"          // pb_requests_get
#include "pb_config_prot.h"          // pb_config_t
#include "pb_devices_prot.h"        // pb_devices_get_number_active
#include "pb_requests_prot.h"       // pb_requests_get
#include "pb_utils.h"          // pb_requests_get
#include "pushbullet.h"         // pb_config_t, pb_config_get_token_key

#ifndef NDEBUG
/**
 * @brief      Dumps all user informations.
 *
 * @param[in]  user  The user
 */
static void _dump_user_info(const pb_user_t);
#endif

static void user_json_to_flat(JsonObject*, const gchar*, JsonNode *, gpointer);


pb_user_t* pb_user_new(void)
{
    pb_user_t* u = calloc(1, sizeof(pb_user_t));

    if (u)
    {
        // Increase the reference counter
        u->ref++;
    }

    return u;
}


int pb_user_get_ref(const pb_user_t* p_user)
{
    return (p_user) ? p_user->ref : -1;
}


int pb_user_ref(pb_user_t* p_user)
{
    if (!p_user)
    {
        return -1;
    }

    p_user->ref++;
    return 0;
}


int pb_user_unref(pb_user_t* p_user)
{
    if (!p_user)
    {
        return -1;
    }

    if (--p_user->ref <= 0)
    {
        /* Do not remove user->token_key because it causes a munmap_chunk since the token_key is given as an argument of
         * the program.
         * The free() function frees the memory space pointed to by ptr, which must have been returned by a previous
         * call to malloc(), calloc() or realloc(). Otherwise, or if free(ptr) has already been called before, undefined
         * behavior occurs. If ptr is NULL, no operation is performed.
         * pb_free(user->token_key);
         */
        p_user->active            = 0;
        p_user->created           = 0;
        p_user->modified          = 0;
        p_user->max_upload_size   = 0;

        pb_free(p_user->email);
        pb_free(p_user->email_normalized);
        pb_free(p_user->iden);
        pb_free(p_user->image_url);
        pb_free(p_user->name);
        pb_config_unref(p_user->config);
        pb_devices_unref(p_user->devices);

        free(p_user);
    }

    return 0;
}


unsigned char pb_user_is_active(const pb_user_t* p_user)
{
    return (p_user) ? p_user->active : 0;
}


char* pb_user_get_name(const pb_user_t* p_user)
{
    return (p_user) ? p_user->name : NULL;
}


char* pb_user_get_email(const pb_user_t* p_user)
{
    return (p_user) ? p_user->email : NULL;
}


char* pb_user_get_iden(const pb_user_t* p_user)
{
    return (p_user) ? p_user->iden : NULL;
}


int pb_user_set_config(pb_user_t* p_user,
                       pb_config_t* p_config)
{
    if ( (! p_user) || (! p_config) )
    {
        return -1;
    }

    p_user->config = p_config;
    pb_config_ref(p_config);

    return 0;
}


pb_config_t* pb_user_get_config(const pb_user_t* p_user)
{
    return ( p_user ) ? p_user->config : NULL;
}


http_code_t pb_user_get_info(pb_user_t *p_user)
{
    char *result       = NULL;
    size_t result_sz   = 0;
    JsonParser *parser = json_parser_new();
    http_code_t res    = HTTP_UNKNOWN_CODE;
    JsonNode *root     = NULL;
    JsonObject *obj    = NULL;
    GError* err = NULL;


    // Access the API using the token
    res = pb_requests_get(&result, &result_sz, API_URL_ME, (pb_config_t*) pb_user_get_config(p_user));

    if ( (res == HTTP_OK) && (json_parser_load_from_data(parser, result, result_sz, NULL)) )
    {
        root = json_parser_get_root(parser);
        obj = json_node_get_object(root);

        json_object_foreach_member(obj, user_json_to_flat, p_user);

        #ifndef NDEBUG
        _dump_user_info(*p_user);
        #endif
    }

    if (err) g_error_free(err);
    g_object_unref(parser);
    pb_free(result);

    return (res);
}


http_code_t pb_user_retrieve_devices(pb_user_t *user)
{
    // CURL results
    char *result = NULL;
    size_t result_sz = 0;
    unsigned short res = 0;


    res = pb_requests_get(&result, &result_sz, API_URL_DEVICES, (pb_config_t*) pb_user_get_config(user));

    // If we do not have a 200 OK, we stop the function and we return the HTTP Status code
    if ( res == HTTP_OK )
    {
        // Create the new list and set it into the
        pb_devices_t *devices = pb_devices_new();

        // Free the old list of devices
        pb_user_unref_devices(user);

        // Set the new list
        pb_user_set_devices(user, devices);

        // Parse and store the new datas
        pb_devices_load_devices_from_data(devices, result, result_sz);
    }

    pb_free(result);

    return (res);
}


pb_devices_t* pb_user_get_devices(const pb_user_t* p_user)
{
    return ( p_user ) ? p_user->devices : NULL;
}


int pb_user_set_devices(pb_user_t* p_user,
                        pb_devices_t* p_devices)
{
    if ( (! p_user) || (! p_devices) )
    {
        return -1;
    }

    p_user->devices = p_devices;
    return 0;
}


size_t pb_user_get_number_active_devices(const pb_user_t *p_user)
{
    return (p_user) ? pb_devices_get_number_active(p_user->devices) : 0;
}


const char* pb_user_get_device_iden_from_name(const pb_user_t *p_user,
                                              const char* nickname)
{
    return (p_user && nickname) ? pb_devices_get_iden_from_name(p_user->devices, nickname) : NULL;
}


int pb_user_unref_devices(const pb_user_t *p_user)
{
    return (p_user) ? pb_devices_unref(p_user->devices) : -1;
}


#ifndef NDEBUG
static void _dump_user_info(const pb_user_t user)
{
    iprintf(" %s - %s", user.name, user.email);
    // iprintf("\ttoken_key : %s", pb_config_get_token_key(user.config));
    iprintf("\tactive : %u", user.active);
    iprintf("\tcreated : %f", user.created);
    iprintf("\tmodified : %f", user.modified);
    iprintf("\temail_normalized : %s", user.email_normalized);
    iprintf("\tiden : %s", user.iden);
    iprintf("\timage_url : %s", user.image_url);
    iprintf("\tmax_upload_size : %d", user.max_upload_size);
}
#endif

static void user_json_to_flat(JsonObject *object __attribute__((unused)),
                          const gchar *member_name,
                          JsonNode *member_node,
                          gpointer userdata
                          )
{
    pb_user_t* user = (pb_user_t*) userdata;

    if (JSON_NODE_HOLDS_VALUE(member_node))
    {
        JSON_ASSOCIATE_INT(user, active);
        JSON_ASSOCIATE_DOUBLE(user, created);
        JSON_ASSOCIATE_DOUBLE(user, modified);
        JSON_ASSOCIATE_STR(user, email);
        JSON_ASSOCIATE_STR(user, email_normalized);
        JSON_ASSOCIATE_STR(user, iden);
        JSON_ASSOCIATE_STR(user, image_url);
        JSON_ASSOCIATE_STR(user, name);
        JSON_ASSOCIATE_INT(user, max_upload_size);
    }
}
