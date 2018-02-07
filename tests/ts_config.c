#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "lib/pb_config_priv.h"
#include "pushbullet.h"

static void test_empty_config(void)
{
    pb_config_t* c = pb_config_new();

    g_assert( c != NULL );
    g_assert( c->ref == 1 );
    
    pb_config_unref(c);
    g_assert( c->ref == 0 );
}

static void test_from_env(void)
{
    pb_config_t* c = NULL;
    struct {
        char* https_proxy;
        char* http_proxy;
        char* token_key;
    } old_env;

    // Get the old values
    old_env.https_proxy = (getenv(HTTPS_PROXY_KEY_ENV)) ? strdup(getenv(HTTPS_PROXY_KEY_ENV)) : NULL;
    old_env.http_proxy = (getenv(HTTP_PROXY_KEY_ENV)) ? strdup(getenv(HTTP_PROXY_KEY_ENV)) : NULL;
    old_env.token_key = (getenv(PB_TOKEN_KEY_ENV)) ? strdup(getenv(PB_TOKEN_KEY_ENV)) : NULL;

    // Test HTTPS_PROXY_KEY_ENV
    setenv(HTTPS_PROXY_KEY_ENV, "localhost", 1);
    c = pb_config_new();
    g_assert_cmpstr( pb_config_get_https_proxy(c), ==, "localhost" );
    g_assert_cmpstr( pb_config_get_token_key(c), ==, NULL );
    pb_config_unref(c);
    unsetenv(HTTPS_PROXY_KEY_ENV);

    // Test HTTP_PROXY_KEY_ENV
    setenv(HTTP_PROXY_KEY_ENV, "localhost", 1);
    c = pb_config_new();
    g_assert_cmpstr( pb_config_get_https_proxy(c), ==, "localhost" );
    g_assert_cmpstr( pb_config_get_token_key(c), ==, NULL );
    pb_config_unref(c);
    unsetenv(HTTP_PROXY_KEY_ENV);

    // Test PB_TOKEN_KEY_ENV
    setenv(PB_TOKEN_KEY_ENV, "localhost", 1);
    c = pb_config_new();
    g_assert_cmpstr( pb_config_get_https_proxy(c), ==, NULL );
    g_assert_cmpstr( pb_config_get_token_key(c), ==, "localhost" );
    pb_config_unref(c);
    unsetenv(PB_TOKEN_KEY_ENV);

    // Reset the old values
    if (old_env.https_proxy) setenv(HTTPS_PROXY_KEY_ENV, old_env.https_proxy, 1);
    if (old_env.http_proxy) setenv(HTTP_PROXY_KEY_ENV, old_env.http_proxy, 1);
    if (old_env.token_key) setenv(PB_TOKEN_KEY_ENV, old_env.token_key, 1);
}

static void test_ref_config(void)
{
    pb_config_t* c = NULL;
    g_assert( pb_config_ref(c) == -1 );
    g_assert( pb_config_unref(c) == -1 );
 
    c = pb_config_new();

    g_assert( c != NULL );
    g_assert( c->ref == 1 );
    g_assert( pb_config_ref(c) == 0 );
    g_assert( c->ref == 2 );
    g_assert( pb_config_unref(c) == 0 );
    g_assert( c->ref == 1 );
   
    pb_config_unref(c);
    g_assert( c->ref == 0 );
}

static void test_https_proxy(void)
{
    pb_config_t* c = pb_config_new();
    pb_config_t* d = NULL;

    g_assert( c != NULL );

    g_assert( pb_config_get_https_proxy(c) == NULL );
    g_assert( pb_config_get_https_proxy(d) == NULL );

    g_assert_cmpint( pb_config_set_https_proxy(c, "localhost"), ==,  0 );
    g_assert_cmpint( pb_config_set_https_proxy(d, "localhost"), ==,  -1 );

    g_assert( pb_config_get_https_proxy(c) != NULL );
    g_assert( pb_config_get_https_proxy(d) == NULL );

    g_assert_cmpstr( pb_config_get_https_proxy(c), ==, "localhost" );


    pb_config_unref(c);
}

static void test_timeout(void)
{
    pb_config_t* c = pb_config_new();
    pb_config_t* d = NULL;

    g_assert( c != NULL );

    g_assert_cmpint( pb_config_get_timeout(c), ==,  0 );
    g_assert_cmpint( pb_config_get_timeout(d), ==,  0 );

    g_assert_cmpint( pb_config_set_timeout(c, 5), ==,  0 );
    g_assert_cmpint( pb_config_set_timeout(d, 0), ==,  -1 );

    g_assert_cmpint( pb_config_get_timeout(c), ==,  5 );
    g_assert_cmpint( pb_config_get_timeout(d), ==,  0 );

    pb_config_unref(c);
}

static void test_token_key(void)
{
    pb_config_t* c = pb_config_new();
    pb_config_t* d = NULL;

    g_assert( c != NULL );

    g_assert( pb_config_get_token_key(c) == NULL );
    g_assert( pb_config_get_token_key(d) == NULL );

    g_assert_cmpint( pb_config_set_token_key(c, "hello_world"), ==,  0 );
    g_assert_cmpint( pb_config_set_token_key(d, "hello_world"), ==,  -1 );

    g_assert( pb_config_get_token_key(c) != NULL );
    g_assert( pb_config_get_token_key(d) == NULL );

    g_assert_cmpstr( pb_config_get_token_key(c), ==, "hello_world" );


    pb_config_unref(c);
}

static void test_from_json_file(void)
{
    pb_config_t* c = NULL;

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, NULL), ==,  -1);
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/"), ==,  -1);
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/null.json"), ==,  -1);
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/empty.json"), ==,  -1);
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/empty_json.json") , ==, 0);
    g_assert( pb_config_get_token_key(c) == NULL );
    g_assert( pb_config_get_https_proxy(c) == NULL );
    g_assert_cmpint( pb_config_get_timeout(c), ==,  0 );
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/empty_values.json") , ==, 0);
    g_assert( pb_config_get_token_key(c) != NULL );
    g_assert( pb_config_get_https_proxy(c) != NULL );
    g_assert_cmpint( strlen(pb_config_get_token_key(c)), ==,  0 );
    g_assert_cmpint( strlen(pb_config_get_https_proxy(c)), ==,  0 );
    g_assert_cmpint( pb_config_get_timeout(c), ==,  0 );
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/proxies.json") , ==, 0);
    g_assert( pb_config_get_token_key(c) == NULL );
    g_assert( pb_config_get_https_proxy(c) != NULL );
    g_assert_cmpint( strlen(pb_config_get_https_proxy(c)), !=, 0 );
    g_assert_cmpstr( pb_config_get_https_proxy(c), ==, "localhost" );
    g_assert_cmpint( pb_config_get_timeout(c), ==,  0 );
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/timeout.json") , ==, 0);
    g_assert( pb_config_get_token_key(c) == NULL );
    g_assert( pb_config_get_https_proxy(c) == NULL );
    g_assert_cmpint( pb_config_get_timeout(c), ==,  2 );
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/token_key.json") , ==, 0);
    g_assert( pb_config_get_https_proxy(c) == NULL );
    g_assert( pb_config_get_token_key(c) != NULL );
    g_assert_cmpint( strlen(pb_config_get_token_key(c)), !=, 0 );
    g_assert_cmpstr( pb_config_get_token_key(c), ==, "toto_is_in_da_place!" );
    g_assert_cmpint( pb_config_get_timeout(c), ==,  0 );
    pb_config_unref(c);

    c = pb_config_new();
    g_assert( c != NULL );
    g_assert_cmpint(pb_config_from_json_file(c, "conf/all.json") , ==, 0);
    g_assert( pb_config_get_https_proxy(c) != NULL );
    g_assert_cmpint( strlen(pb_config_get_https_proxy(c)), !=, 0 );
    g_assert_cmpstr( pb_config_get_https_proxy(c), ==, "localhost" );
    g_assert( pb_config_get_token_key(c) != NULL );
    g_assert_cmpint( strlen(pb_config_get_token_key(c)), !=, 0 );
    g_assert_cmpstr( pb_config_get_token_key(c), ==, "toto_is_in_da_place!" );
    g_assert_cmpint( pb_config_get_timeout(c), ==,  2 );
    pb_config_unref(c);
}


int main (int argc, char *argv[])
{
    g_test_init (&argc, &argv, NULL);

    g_test_add_func("/config/empty", test_empty_config);
    g_test_add_func("/config/from-env", test_from_env);
    g_test_add_func("/config/ref", test_ref_config);
    g_test_add_func("/config/set-get-https-proxy", test_https_proxy);
    g_test_add_func("/config/set-get-timeout", test_timeout);
    g_test_add_func("/config/set-get-token-key", test_token_key);
    g_test_add_func("/config/from-json-file", test_from_json_file);

    return g_test_run ();
}