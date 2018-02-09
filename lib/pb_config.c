/**
 * @file pb_config.c
 * @author hbuyse
 * @date 29/12/2017
 */

#include <stdint.h>     // uint32_t, int32_t
#include <string.h>     // strdup
#include <stdlib.h>     // getenv
#include <pthread.h>    // pthread_mutex_t, PTHREAD_MUTEX_INITIALIZER, pthread_mutex_lock, pthread_mutex_unlock
#include <json-glib/json-glib.h>    // JsonObject, JsonNode, GError, json_parser_new, json_parser_load_from_file, 
                                    // json_object_new, json_parser_get_root

#include "pb_config_priv.h"     // pb_config_t, HTTP_PROXY_KEY_ENV, HTTPS_PROXY_KEY_ENV, PB_TOKEN_KEY_ENV
#include "pb_config_prot.h"     // HTTP_PROXY_KEY_ENV, HTTPS_PROXY_KEY_ENV, PB_TOKEN_KEY_ENV
#include "pb_utils.h"        // eprintf, gprintf, pb_free
#include "pushbullet.h"


pb_config_t* pb_config_new(void)
{
    pb_config_t* p_config = calloc(1, sizeof(pb_config_t));

    if ( p_config )
    {
        pthread_mutex_init(&p_config->mtx, NULL);

        // Increase the reference
        p_config->ref++;

        // Check the environment variable https_proxy then http_proxy
        pb_config_set_proxy(p_config, getenv(HTTPS_PROXY_KEY_ENV));

        if ( ! p_config->proxy )
        {
            pb_config_set_proxy(p_config, getenv(HTTP_PROXY_KEY_ENV));
        }

        pb_config_set_token_key(p_config, getenv(PB_TOKEN_KEY_ENV));
    }

    return p_config;
}

int pb_config_lock(pb_config_t* p_config)
{
    if ( ! p_config )
    {
        return -1;
    }

    return pthread_mutex_lock(&p_config->mtx);
}

int pb_config_unlock(pb_config_t* p_config)
{
    if ( ! p_config )
    {
        return -1;
    }

    return pthread_mutex_unlock(&p_config->mtx);
}

int pb_config_ref(pb_config_t* p_config)
{
    if ( ! p_config )
    {
        return -1;
    }

    pthread_mutex_lock(&p_config->mtx);

    p_config->ref++;

    pthread_mutex_unlock(&p_config->mtx);

    return 0;
}


int pb_config_unref(pb_config_t* p_config)
{
    if ( ! p_config )
    {
        return -1;
    }

    pthread_mutex_lock(&p_config->mtx);

    if (--p_config->ref <= 0)
    {
        pb_free(p_config->proxy);
        pb_free(p_config->token_key);
        free(p_config);
    }

    pthread_mutex_unlock(&p_config->mtx);

    return 0;
}


int pb_config_set_proxy(pb_config_t* p_config, const char* proxy)
{
    if ( ! p_config )
    {
        return -1;
    }

    pthread_mutex_lock(&p_config->mtx);

    // Free the field and put it to NULL
    pb_free(p_config->proxy);

    // If there is a new value, we set it
    if ( proxy )
    {
        p_config->proxy = strdup(proxy);
    }

    pthread_mutex_unlock(&p_config->mtx);

    return 0;
}


int pb_config_set_timeout(pb_config_t* p_config, const long timeout)
{
    if ( ! p_config )
    {
        return -1;
    }

    pthread_mutex_lock(&p_config->mtx);

    p_config->timeout = (timeout < 0) ? 0 : timeout;

    pthread_mutex_unlock(&p_config->mtx);

    return 0;
}


int pb_config_set_token_key(pb_config_t* p_config, const char* token_key)
{
    if ( ! p_config )
    {
        return -1;
    }

    pthread_mutex_lock(&p_config->mtx);

    // Free the field and put it to NULL
    pb_free(p_config->token_key);

    // If there is a new value, we set it
    if ( token_key )
    {
        p_config->token_key = strdup(token_key);
    }

    pthread_mutex_unlock(&p_config->mtx);

    return 0;
}


int pb_config_get_ref(const pb_config_t* p_config)
{
    return ( p_config ) ? p_config->ref : 0;
}


char* pb_config_get_proxy(const pb_config_t* p_config)
{
    return (p_config) ? p_config->proxy : NULL;
}


long pb_config_get_timeout(const pb_config_t* p_config)
{
    return (p_config) ? p_config->timeout : 0;
}


char* pb_config_get_token_key(const pb_config_t* p_config)
{
    return (p_config) ? p_config->token_key : NULL;
}


int pb_config_from_json_file(pb_config_t* p_config, const char *json_filepath)
{
    int ret = -1;
    JsonParser *parser = NULL;
    GError* err = NULL;
    JsonNode *node = NULL;
    JsonObject* obj = NULL;

    if ( p_config && json_filepath )
    {
        parser = json_parser_new();

        if ( ! json_parser_load_from_file(parser, json_filepath, &err) )
        {
            #ifdef __TRACES__
            eprintf("%s\n", err->message);
            #endif
            g_clear_error(&err);
        }
        else
        {
            node = json_parser_get_root(parser);

            // check if it is an JsonObject inside
            if ( (! node) || (! JSON_NODE_HOLDS_OBJECT(node)) )
            {
                #ifdef __TRACES__
                eprintf("json_filepath does not contain a valid JSON object");
                #endif
            }
            else
            {
                obj = json_node_get_object(node);

                if (obj)
                {
                    // Before, check if timeout is superior than long_max
                    if (json_object_has_member(obj, "timeout"))
                    {
                        pb_config_set_timeout(p_config, (const long) json_object_get_int_member(obj, "timeout"));
                    }

                    if (json_object_has_member(obj, "proxy"))
                    {
                        pb_config_set_proxy(p_config, strdup(json_object_get_string_member(obj, "proxy")));
                    }

                    if (json_object_has_member(obj, "token_key"))
                    {
                        pb_config_set_token_key(p_config, strdup(json_object_get_string_member(obj, "token_key")));
                    }

                    ret = 0;
                }
            }

            #ifdef __TRACES__
            print_json_node_to_stream(gprintf, node);
            #endif
        }

        // Free the parser
        g_object_unref(parser);
    }

    return ret;
}