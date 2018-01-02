#include <stdlib.h>
#include <check.h>

#include "pushbullet.h"

START_TEST (test_pb_config_new)
{
    pb_config_t* c = pb_config_new();

    ck_assert( c != NULL );
    
    if (c) { free(c); }
}
END_TEST

START_TEST (test_pb_config_ref_unref)
{
    pb_config_t* c = pb_config_new();

    ck_assert( c != NULL );

    pb_config_ref(c);
    ck_assert( c != NULL );

    pb_config_unref(c);
    ck_assert( c != NULL );

    pb_config_unref(c);
    ck_assert( c != NULL );
    
    pb_config_unref(c);
    ck_assert( c != NULL );
}
END_TEST

START_TEST (test_pb_config_https_proxy)
{
    pb_config_t* c = pb_config_new();
    pb_config_t* d = NULL;

    ck_assert( c != NULL );

    ck_assert( pb_config_get_https_proxy(c) == NULL );
    ck_assert( pb_config_get_https_proxy(d) == NULL );

    ck_assert_int_eq( pb_config_set_https_proxy(c, "localhost"), 0 );
    ck_assert_int_eq( pb_config_set_https_proxy(d, "localhost"), -1 );

    ck_assert( pb_config_get_https_proxy(c) != NULL );
    ck_assert( pb_config_get_https_proxy(d) == NULL );

    ck_assert_str_eq( pb_config_get_https_proxy(c), "localhost" );


    pb_config_unref(c);
}
END_TEST

START_TEST (test_pb_config_timeout)
{
    pb_config_t* c = pb_config_new();
    pb_config_t* d = NULL;

    ck_assert( c != NULL );

    ck_assert_int_eq( pb_config_get_timeout(c), 0 );
    ck_assert_int_eq( pb_config_get_timeout(d), 0 );

    ck_assert_int_eq( pb_config_set_timeout(c, 5), 0 );
    ck_assert_int_eq( pb_config_set_timeout(d, 0), -1 );

    ck_assert_int_eq( pb_config_get_timeout(c), 5 );
    ck_assert_int_eq( pb_config_get_timeout(d), 0 );

    pb_config_unref(c);
}
END_TEST

START_TEST (test_pb_config_token_key)
{
    pb_config_t* c = pb_config_new();
    pb_config_t* d = NULL;

    ck_assert( c != NULL );

    ck_assert( pb_config_get_token_key(c) == NULL );
    ck_assert( pb_config_get_token_key(d) == NULL );

    ck_assert_int_eq( pb_config_set_token_key(c, "hello_world"), 0 );
    ck_assert_int_eq( pb_config_set_token_key(d, "hello_world"), -1 );

    ck_assert( pb_config_get_token_key(c) != NULL );
    ck_assert( pb_config_get_token_key(d) == NULL );

    ck_assert_str_eq( pb_config_get_token_key(c), "hello_world" );


    pb_config_unref(c);
}
END_TEST

START_TEST (test_pb_config_from_json_file)
{
    pb_config_t* c = NULL;

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, NULL), -1);
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/"), -1);
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/null.json"), -1);
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/empty.json"), -1);
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/empty_json.json"), 0);
    ck_assert( pb_config_get_token_key(c) == NULL );
    ck_assert( pb_config_get_https_proxy(c) == NULL );
    ck_assert_int_eq( pb_config_get_timeout(c), 0 );
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/empty_values.json"), 0);
    ck_assert( pb_config_get_token_key(c) != NULL );
    ck_assert( pb_config_get_https_proxy(c) != NULL );
    ck_assert_int_eq( strlen(pb_config_get_token_key(c)), 0 );
    ck_assert_int_eq( strlen(pb_config_get_https_proxy(c)), 0 );
    ck_assert_int_eq( pb_config_get_timeout(c), 0 );
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/proxies.json"), 0);
    ck_assert( pb_config_get_token_key(c) == NULL );
    ck_assert( pb_config_get_https_proxy(c) != NULL );
    ck_assert_int_ne( strlen(pb_config_get_https_proxy(c)), 0 );
    ck_assert_str_eq( pb_config_get_https_proxy(c), "localhost" );
    ck_assert_int_eq( pb_config_get_timeout(c), 0 );
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/timeout.json"), 0);
    ck_assert( pb_config_get_token_key(c) == NULL );
    ck_assert( pb_config_get_https_proxy(c) == NULL );
    ck_assert_int_eq( pb_config_get_timeout(c), 2 );
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/token_key.json"), 0);
    ck_assert( pb_config_get_https_proxy(c) == NULL );
    ck_assert( pb_config_get_token_key(c) != NULL );
    ck_assert_int_ne( strlen(pb_config_get_token_key(c)), 0 );
    ck_assert_str_eq( pb_config_get_token_key(c), "toto_is_in_da_place!" );
    ck_assert_int_eq( pb_config_get_timeout(c), 0 );
    pb_config_unref(c);

    c = pb_config_new();
    ck_assert( c != NULL );
    ck_assert_int_eq(pb_config_from_json_file(c, "conf/all.json"), 0);
    ck_assert( pb_config_get_https_proxy(c) != NULL );
    ck_assert_int_ne( strlen(pb_config_get_https_proxy(c)), 0 );
    ck_assert_str_eq( pb_config_get_https_proxy(c), "localhost" );
    ck_assert( pb_config_get_token_key(c) != NULL );
    ck_assert_int_ne( strlen(pb_config_get_token_key(c)), 0 );
    ck_assert_str_eq( pb_config_get_token_key(c), "toto_is_in_da_place!" );
    ck_assert_int_eq( pb_config_get_timeout(c), 2 );
    pb_config_unref(c);
}
END_TEST

// pb_config_from_json_file

Suite * pb_config_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Config");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_pb_config_new);
    tcase_add_test(tc_core, test_pb_config_ref_unref);
    tcase_add_test(tc_core, test_pb_config_https_proxy);
    tcase_add_test(tc_core, test_pb_config_timeout);
    tcase_add_test(tc_core, test_pb_config_token_key);
    tcase_add_test(tc_core, test_pb_config_from_json_file);

    suite_add_tcase(s, tc_core);

    return s;
}