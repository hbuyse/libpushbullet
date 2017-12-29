#include <stdint.h>     // uint32_t, int32_t
#include <string.h>     // strdup
#include <json-glib/json-glib.h>    // JsonObject, JsonNode, GError, json_parser_new, json_parser_load_from_file, 
                                    // json_object_new, json_parser_get_root

#include "pb_utils.h"        // eprintf, gprintf, pb_free


typedef struct pb_config_s {
    char* https_proxy;             ///< HTTPS proxy
    long  timeout;             ///< CURL timeout
    char* token_key;             ///< Pushbullet token key
    int   ref_count;          ///< Reference count
} pb_config_t;


pb_config_t* pb_config_new(void)
{
    return calloc(1, sizeof(pb_config_t));
}


void pb_config_ref(pb_config_t* p_config)
{
    if (!p_config)
    {
        return;
    }

    p_config->ref_count++;
}


void pb_config_unref(pb_config_t* p_config)
{
    if (!p_config)
    {
        return;
    }

    if (--p_config->ref_count <= 0)
    {
        pb_free(p_config->https_proxy);
        pb_free(p_config->token_key);
        free(p_config);
    }
}


int pb_config_set_https_proxy(pb_config_t* p_config, const char* https_proxy)
{
    if (!p_config)
    {
        return -1;
    }

    p_config->https_proxy = strdup(https_proxy);
    return 0;
}


int pb_config_set_timeout(pb_config_t* p_config, const long timeout)
{
    if (!p_config)
    {
        return -1;
    }

    p_config->timeout = (timeout < 0) ? 0 : timeout;
    return 0;
}


int pb_config_set_token_key(pb_config_t* p_config, const char* token_key)
{
    if (!p_config)
    {
        return -1;
    }

    p_config->token_key = strdup(token_key);
    return 0;
}


char* pb_config_get_https_proxy(const pb_config_t* p_config)
{
    return (p_config) ? p_config->https_proxy : NULL;
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

    if (p_config)
    {
        parser = json_parser_new();

        if ( ! json_parser_load_from_file(parser, json_filepath, &err) )
        {
            #ifdef __DEBUG__
            eprintf("%s\n", err->message);
            #endif
            g_clear_error(&err);
        }
        else
        {
            node = json_parser_get_root(parser);

            // check if it is an JsonObject inside
            if ( ! JSON_NODE_HOLDS_OBJECT(node))
            {
                eprintf("json_filepath does not contain a valid JSON object");
            }
            else
            {
                obj = json_node_get_object(node);

                // Before, check if timeout is superior than long_max
                pb_config_set_timeout(p_config, (const long) json_object_get_int_member(obj, "timeout"));
                pb_config_set_https_proxy(p_config, strdup(json_object_get_string_member(obj, "https_proxy")));
                pb_config_set_token_key(p_config, strdup(json_object_get_string_member(obj, "token_key")));

                ret = 0;
            }

            #ifdef __DEBUG__
            print_json_node_to_stream(gprintf, node);
            #endif

            json_object_unref(obj);
        }

        // Free the parser
        g_object_unref(parser);
    }

    return ret;
}