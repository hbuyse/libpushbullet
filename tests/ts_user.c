#include <stdlib.h>

#include <glib.h>
#include <glib/gi18n.h>

#include "lib/pb_user_priv.h"
#include "lib/pb_user_prot.h"
#include "pushbullet.h"

static void test_empty_user(void)
{
    pb_user_t* u = pb_user_new();

    g_assert( u != NULL );
    
    pb_user_unref(u);
}

static void test_config_to_user(void)
{
    pb_user_t* u = pb_user_new();
    pb_config_t* c1 = NULL;
    pb_config_t* c2 = NULL;

    g_assert( u != NULL );
    
    g_assert( pb_user_get_config(u) == NULL );
    g_assert( pb_user_set_config(u, c1) == -1 );

    c1 = pb_config_new();
    pb_config_from_json_file(c1, "conf/all.json");
    g_assert( pb_user_set_config(u, c1) == 0 );

    c2 = pb_user_get_config(u);
    g_assert( c2 != NULL );
    g_assert( c1 == c2 );

    pb_user_unref(u);
}

static void test_get_user_info(void)
{
    pb_user_t* u = pb_user_new();

    g_assert( pb_user_get_info(u) == HTTP_UNAUTHORIZED );
}

static void test_set_devices(void)
{
    pb_user_t* u = pb_user_new();
    pb_devices_t* d = pb_devices_new();

    g_assert( pb_user_set_devices(NULL, NULL) == -1);
    g_assert( pb_user_set_devices(u, NULL) == -1);
    g_assert( pb_user_set_devices(NULL, d) == -1);
    g_assert( pb_user_set_devices(u, d) == 0);

    pb_user_unref(u);
}

static void test_get_devices(void)
{
    pb_user_t* u = pb_user_new();
    pb_devices_t* d = pb_devices_new();

    g_assert( pb_user_get_devices(NULL) == NULL);
    g_assert( pb_user_get_devices(u) == NULL);

    pb_user_set_devices(u, d);

    g_assert( pb_user_get_devices(u) == d);

    pb_user_unref(u);
}

// http_code_t pb_user_get_info(pb_user_t *p_user);
// pb_device_t* pb_user_get_devices_list(const pb_user_t* p_user);

int main (int argc, char *argv[])
{
    g_test_init (&argc, &argv, NULL);

    g_test_add_func("/user/empty-user", test_empty_user);
    g_test_add_func("/user/config-to-user", test_config_to_user);
    // g_test_add_func("/user/get-user-info", test_get_user_info);

    return g_test_run ();
}