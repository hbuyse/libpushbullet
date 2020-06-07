/**
 * @file pb_config.c
 * @author hbuyse
 * @date 29/12/2017
 */

#include "config.h"

#include "pb_utils.h"        // eprintf, gprintf, pb_free
#include "pushbullet.h"


#define HTTP_PROXY_KEY_ENV  "http_proxy"
#define HTTPS_PROXY_KEY_ENV "proxy"
#define PB_TOKEN_KEY_ENV "PB_TOKEN_KEY"

struct pb_config_s {
    char* proxy;             ///< HTTP/HTTPS proxy
    long  timeout;             ///< CURL timeout
    char* token_key;             ///< Pushbullet token key
    int   ref;          ///< Reference count
};

struct pb_config_s* pb_config_new(void)
{
    struct pb_config_s* p_config = calloc(1, sizeof(*p_config));

    if ( p_config )
    {
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

int pb_config_copy(struct pb_config_s* p_dst, struct pb_config_s* p_src)
{
    if ( (! p_dst) || (! p_src) )
    {
        return -1;
    }

    if ( memmove(p_dst, p_src, sizeof(struct pb_config_s)) )
    {
        return -2;
    }

    // Put the destination reference counter to one
    p_dst->ref = 1;

    return 0;
}

int pb_config_ref(struct pb_config_s* p_config)
{
    if ( ! p_config )
    {
        return -1;
    }

    p_config->ref++;

    return 0;
}


int pb_config_unref(struct pb_config_s* p_config)
{
    if ( ! p_config )
    {
        return -1;
    }

    if (--p_config->ref <= 0)
    {
        pb_free(p_config->proxy);
        pb_free(p_config->token_key);
        free(p_config);
    }

    return 0;
}


int pb_config_set_proxy(struct pb_config_s* p_config, const char* proxy)
{
    if ( ! p_config )
    {
        return -1;
    }

    // Free the field and put it to NULL
    pb_free(p_config->proxy);

    // If there is a new value, we set it
    if ( proxy )
    {
        p_config->proxy = strdup(proxy);
    }

    return 0;
}


int pb_config_set_timeout(struct pb_config_s* p_config, const long timeout)
{
    if ( ! p_config )
    {
        return -1;
    }

    p_config->timeout = (timeout < 0) ? 0 : timeout;

    return 0;
}


int pb_config_set_token_key(struct pb_config_s* p_config, const char* token_key)
{
    if ( ! p_config )
    {
        return -1;
    }

    // Free the field and put it to NULL
    pb_free(p_config->token_key);

    // If there is a new value, we set it
    if ( token_key )
    {
        p_config->token_key = strdup(token_key);
    }

    return 0;
}


int pb_config_get_ref(const struct pb_config_s* p_config)
{
    return ( p_config ) ? p_config->ref : 0;
}


char* pb_config_get_proxy(const struct pb_config_s* p_config)
{
    return (p_config) ? p_config->proxy : NULL;
}


long pb_config_get_timeout(const struct pb_config_s* p_config)
{
    return (p_config) ? p_config->timeout : 0;
}


char* pb_config_get_token_key(const struct pb_config_s* p_config)
{
    return (p_config) ? p_config->token_key : NULL;
}


int pb_config_from_json_file(struct pb_config_s* p_config, const char *cfg_path)
{
    int ret = -1;
    config_t* cfg = NULL;


    if ( p_config && cfg_path )
    {
        config_init(cfg);

        if ( ! config_read_file(cfg, cfg_path) )
        {
            #ifndef NDEBUG
            eprintf("%s\n", config_error_text(cfg));
            #endif
        }
        else
        {
            const char* proxy = NULL;
            const char* token = NULL;
            int timeout = 0;

            if (config_lookup_int(cfg, "timeout", &timeout)) {
                pb_config_set_timeout(p_config, (long) timeout);
            }

            if (config_lookup_string(cfg, "proxy", &proxy)) {
                pb_config_set_proxy(p_config, strdup(proxy));
            }

            if (config_lookup_string(cfg, "token", &token)) {
                pb_config_set_token_key(p_config, strdup(token));
            }

            #ifndef NDEBUG
            iprintf("timeout : %ld\n", pb_config_get_timeout(p_config));
            iprintf("proxy   : %s\n", pb_config_get_proxy(p_config));
            iprintf("token   : %s\n", pb_config_get_token_key(p_config));
            #endif

            ret = 0;
        }
    }

    if (cfg)
        config_destroy(cfg);

    return ret;
}
