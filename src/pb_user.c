/**
 * @file user.c
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

#include "pb_internal.h"          // pb_requests_get
#include "pb_utils.h"          // pb_requests_get
#include "pushbullet.h"         // pb_config_t, pb_config_get_token_key
// #include "devices.h"          // pb_free_devices


/**
 * @brief Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000



/**
 * @struct pb_user_s
 * @brief Contains the user informations.
 */
struct pb_user_s {
    unsigned char active;          ///< Boolean that tells if the user is active or not
    double created;             ///< Epoch time when the user was created
    double modified;            ///< Epoch time when the user was last modified
    const char *email;          ///< The user's email
    const char *email_normalized;          ///< The user's email normalized
    const char *iden;           ///< The user's identification
    const char *image_url;          ///< The URL to the user's photo
    const char *name;           ///< The user's name
    int max_upload_size;          ///< The maximum size of a file the user can upload in bytes
    pb_config_t *config;            ///< Configuration from the config file
    pb_device_t *devices;          ///< The list of active devices
    int32_t     ref_count;          ///< Reference count
};

#ifdef __DEBUG__
/**
 * @brief      Dumps all user informations.
 *
 * @param[in]  user  The user
 */
static void _dump_user_info(const pb_user_t);
#endif

static void _json_to_flat(JsonObject*, const gchar*, JsonNode *, gpointer);


pb_user_t* pb_user_new(void)
{
    return calloc(1, sizeof(pb_user_t));
}


int pb_user_set_config(pb_user_t* p_user, pb_config_t* p_config)
{
    if ( (! p_user) || (! p_config) )
    {
        return -1;
    }

    p_user->config = p_config;
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

    if ( (res == HTTP_OK) && (json_parser_load_from_data(parser, result, result_sz, &err)) )
    {
        root = json_parser_get_root(parser);
        obj = json_node_get_object(root);

        json_object_foreach_member(obj, _json_to_flat, p_user);

        #ifdef __DEBUG__
        _dump_user_info(*p_user);
        #endif
    }
    
    pb_free(result);

    return (res);
}


pb_device_t* pb_user_get_devices_list(const pb_user_t* p_user)
{
    return ( p_user ) ? p_user->devices : NULL;
}


void pb_user_free(pb_user_t *p_user)
{
    if ( p_user )
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

        // pb_free_devices(user);
    }
}

#ifdef __DEBUG__
static void _dump_user_info(const pb_user_t user)
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
    // fprintf(stdout, "\e[1m[%s]\e[0m\tconfig: %s\n", __func__, json_object_to_json_string(user.config) );
}
#endif


/**
 * @brief      Macro to associate a key in a structure
 *
 * @param      type  The type
 * @param      var   The pointer we fill
 * @param      k     The JSON key
 */
#define     JSON_ASSOCIATE(type, var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) {var->k = json_node_get_ ## type(member_node); } } while(0)

static void _json_to_flat(JsonObject *object __attribute__((unused)),
                          const gchar *member_name,
                          JsonNode *member_node,
                          gpointer userdata
                          )
{
    pb_user_t* user = (pb_user_t*) userdata;

    if (JSON_NODE_HOLDS_VALUE(member_node))
    {
        JSON_ASSOCIATE(int, user, active);
        JSON_ASSOCIATE(double, user, created);
        JSON_ASSOCIATE(double, user, modified);
        JSON_ASSOCIATE(string, user, email);
        JSON_ASSOCIATE(string, user, email_normalized);
        JSON_ASSOCIATE(string, user, iden);
        JSON_ASSOCIATE(string, user, image_url);
        JSON_ASSOCIATE(string, user, name);
        JSON_ASSOCIATE(int, user, max_upload_size);    
    }
}