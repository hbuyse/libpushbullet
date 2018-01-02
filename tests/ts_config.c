#include <stdlib.h>
#include <string.h>

#include "pushbullet.h"

static void test_empty_config(void)
{
    pb_config_t* c = pb_config_new();

    g_assert( c != NULL );
    
    pb_config_unref(c);
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

    g_test_add_func("/user/empty-config", test_empty_config);
    g_test_add_func("/user/set-get-https-proxy", test_https_proxy);
    g_test_add_func("/user/set-get-timeout", test_timeout);
    g_test_add_func("/user/set-get-token-key", test_token_key);
    g_test_add_func("/user/from-json-file", test_from_json_file);

    return g_test_run ();
}