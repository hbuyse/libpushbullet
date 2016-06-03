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

#include <json/json.h>          // json_object, json_tokener_parse, json_object_get_string, json_object_object_foreach,
                                // json_object_get_int

#include <urls.h>           // API_URL_ME
#include <requests.h>          // pb_get
#include <pb_structures.h>          // pb_user_t
#include <http_code.h>          // HTTP_OK
#include <devices.h>          // pb_free_devices


/**
 * @brief Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000


/**
 * @brief      Macro to associate a key in a structure
 *
 * @param      type  The type
 * @param      var   The pointer we fill
 * @param      k     The JSON key
 */
#define     JSON_ASSOCIATE(type, var, k)          \
    if ( strcmp(key, # k) == 0 ) {var->k = json_object_get_ ## type(val); }


/**
 * @brief      Free a pointer if it exists
 *
 * @param      ptr   The pointer
 */
#define     FREE(ptr)                             \
    if ( ptr ) {free( (void *) ptr); ptr = NULL; }


/**
 * @brief      Dumps all user informations.
 *
 * @param[in]  user  The user
 */
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
    fprintf(stdout, "\e[1m[%s]\e[0m\tconfig: %s\n", __func__, json_object_to_json_string(user.config) );
}



/**
 * @brief      Determines if path is a regular file
 *
 * @param[in]  path  The path to the file
 *
 * @return     True if regular file, False otherwise.
 */
static unsigned char _is_regular_file(const char *path)
{
    struct stat     path_stat;


    // Get tje stats about the file
    stat(path, &path_stat);


    // This macro returns non-zero if the file is a regular file.
    return (S_ISREG(path_stat.st_mode) );
}



unsigned short pb_get_user_info(pb_user_t           *user,
                                const char          *token_key,
                                const json_object   *config
                                )
{
    char                *result     = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    unsigned short      res         = 0;
    json_object         *json_obj   = NULL;


    // Copy the token
    user->token_key = token_key;
    user->config    = (config) ? ( (json_object *) config) : NULL;


    // Access the API using the token
    res = pb_get(result, API_URL_ME, *user);


    #ifdef __DEBUG__
    fprintf( (res == HTTP_OK) ? stdout : stderr, "\e[1;3%dm[%s]\e[37m %u\e[0m %s\n", (res == HTTP_OK) ? 2 : 1, __func__, res, result);
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



void pb_free_user(pb_user_t *user)
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
        FREE(user->config);

        pb_free_devices(user);
    }
}



json_object* pb_get_config_json(const char *path)
{
    json_object     *config_json        = NULL;
    FILE            *config_fd          = NULL; // File descriptor to the config file
    size_t          config_file_length  = 0;    // Length of the config file
    char            *config_data        = NULL; // Content of the config file
    size_t          read = 0; // Read bytes from the config file


    if ( path == NULL )
    {
        #ifdef __DEBUG__
        fprintf(stderr, "\e[1;31m[%s]\e[0m No path given for the config file.\n", __func__);
        #endif

        return (NULL);
    }

    if ( ! _is_regular_file(path) )
    {
        #ifdef __DEBUG__
        fprintf(stderr, "\e[1;31m[%s]\e[0m %s is not a file.\n", __func__, path);
        #endif

        return (NULL);
    }


    // Trying to open the config file
    if ( (config_fd = fopen(path, "r") ) == NULL )
    {
        #ifdef __DEBUG__
        fprintf(stderr, "\e[1;31m[%s]\e[0m Could not open the config file at %s: %m", __func__, path);
        fprintf(stderr, "\n");
        #endif

        return (NULL);
    }


    // We get the file of the file
    fseek(config_fd, 0, SEEK_END);
    config_file_length = ftell(config_fd);
    fseek(config_fd, 0, SEEK_SET);

    if ( config_file_length == 0 )
    {
        #ifdef __DEBUG__
        fprintf(stderr, "\e[1;31m[%s]\e[0m %s is an empty file.\n", __func__, path);
        #endif

        return (NULL);
    }


    // We allocate the memory for the contents
    config_data = (char *) calloc(config_file_length + 1, sizeof(char) );


    // Read the file and store it in the memory
    read        = fread(config_data, sizeof(char), config_file_length, config_fd);

    if ( read != config_file_length )
    {
        #ifdef __DEBUG__
        fprintf(stderr,
                "\e[1;31m[%s]\e[0m Read %ld on %ld from the config file %s.\n",
                __func__,
                read,
                config_file_length,
                path);
        #endif
    }


    // Close the file
    fclose(config_fd);


    // Use the JSON parser
    config_json = json_tokener_parse(config_data);

    if ( json_object_object_length(config_json) == 0 )
    {
        #ifdef __DEBUG__
        fprintf(stderr, "\e[1;31m[%s]\e[0m %s is a JSON file, but it has no fields.\n", __func__, path);
        #endif

        return (NULL);
    }

    #ifdef __DEBUG__
    fprintf(stdout, "\e[1;32m[%s]\e[0m %s\n", __func__, json_object_to_json_string(config_json) );
    #endif


    // Free the memory allocated for the
    FREE(config_data);

    return (config_json);
}



const char* pb_get_http_proxy(const pb_user_t user)
{
    if ( user.config == NULL )
    {
        return (NULL);
    }

    json_object_object_foreach(user.config, key, val)
    {
        if ( (strcmp(key, "http_proxy") == 0) )
        {
            const char     *http_proxy = json_object_get_string(val);

            return ( (strlen(http_proxy) != 0) ? http_proxy : NULL);
        }
    }

    return (NULL);
}



const char* pb_get_https_proxy(const pb_user_t user)
{
    if ( user.config == NULL )
    {
        return (NULL);
    }

    json_object_object_foreach(user.config, key, val)
    {
        if ( (strcmp(key, "https_proxy") == 0) )
        {
            const char     *https_proxy = json_object_get_string(val);

            return ( (strlen(https_proxy) != 0) ? https_proxy : NULL);
        }
    }

    return (NULL);
}



int pb_get_curl_timeout(const pb_user_t user)
{
    if ( user.config == NULL )
    {
        return (0);
    }

    json_object_object_foreach(user.config, key, val)
    {
        if ( (strcmp(key, "curl_timeout") == 0) )
        {
            return (json_object_get_int(val) );
        }
    }

    return (0);
}



const char* pb_get_token_key(const json_object *config)
{
    if ( config == NULL )
    {
        return (NULL);
    }

    json_object_object_foreach( (json_object *) config, key, val)
    {
        if ( (strcmp(key, "token_key") == 0) )
        {
            const char     *token_key = json_object_get_string(val);

            return ( (strlen(token_key) != 0) ? token_key : NULL);
        }
    }

    return (NULL);
}