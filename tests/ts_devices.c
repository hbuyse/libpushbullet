#include <glib.h>
#include <glib/gi18n.h>

#include <sys/types.h>   // open, lseek
#include <sys/stat.h>   // open
#include <fcntl.h>   // open
#include <unistd.h>   // lseek
#include <stdio.h>

#include "lib/pb_devices_prot.h"
#include "pushbullet.h"


static void test_empty_devices(void)
{
    pb_devices_t* dl = pb_devices_new();
    pb_devices_ref(dl);

    g_assert_nonnull( dl );
    
    pb_devices_unref(dl);
}

static void test_ref_devices(void)
{
    pb_devices_t* d = NULL;
    g_assert_cmpint( pb_devices_ref(d), ==, -1 );
    g_assert_cmpint( pb_devices_unref(d), ==, -1 );
 
    d = pb_devices_new();

    g_assert_nonnull( d );
    g_assert_cmpint( pb_devices_get_ref(d), ==, 1 );
    g_assert_cmpint( pb_devices_ref(d), ==, 0 );
    g_assert_cmpint( pb_devices_get_ref(d), ==, 2 );
    g_assert_cmpint( pb_devices_unref(d), ==, 0 );
    g_assert_cmpint( pb_devices_get_ref(d), ==, 1 );
   
    pb_devices_unref(d);
    g_assert_cmpint( pb_devices_get_ref(d), ==, 0 );
}

static void test_add_new_device(void)
{
    pb_devices_t* d = pb_devices_new();
    pb_device_t* nd1 = pb_device_new();
    pb_device_t* nd2 = pb_device_new();

    g_assert_cmpint( pb_devices_add_new_device(NULL, NULL), ==, -1);
    g_assert_cmpint( pb_devices_add_new_device(d, NULL), ==, -1);
    g_assert_cmpint( pb_devices_add_new_device(NULL, nd1), ==, -1);

    g_assert_cmpint( pb_devices_add_new_device(d, nd1), ==, 0);
    g_assert_cmpint( pb_devices_get_number_active(d), ==, 1);
    g_assert_cmpint( pb_devices_add_new_device(d, nd2), ==, 0);
    g_assert_cmpint( pb_devices_get_number_active(d), ==, 2);

    pb_devices_unref(d);
    g_assert_cmpint( pb_devices_get_number_active(d), ==, 0);
}

static void test_load_devices_from_string(void)
{
    int i = 0;;
    char not_json[] = "hello world";
    char json[] = "{ \"hello\": \"world\" }";

    struct {
        char* data;
        size_t len;
        int result;
    } tests[] = {
        {.data = NULL, .len = -1, .result = -1},
        {.data = NULL, .len = 0, .result = -1},
        {.data = NULL, .len = 0, .result = -1},
        {.data = NULL, .len = 10, .result = -1},
        {.data = NULL, .len = 10, .result = -1},
        {.data = not_json, .len = sizeof(not_json) / sizeof(not_json[0]), .result = -1},
        {.data = json, .len = sizeof(json) / sizeof(json[0]), .result = 0}
    };



    for (i = 0; i < (sizeof(tests) / sizeof(tests[0])); i++)
    {
        pb_devices_t* d = pb_devices_new();
        g_assert_nonnull( d );
        g_assert_cmpint( pb_devices_load_devices_from_data(d, tests[i].data, tests[i].len), ==, tests[i].result );
        pb_devices_unref(d);
    }

}

static int load_json_from_file(char** result, size_t *len, char* filepath)
{
    int ret = -10;
    int fd = -1;

    if ((!result) || (!len) || (!filepath))
    {
        ret = -1;   
    }
    else if ((fd = open(filepath, O_RDONLY)) == -1)
    {
        ret = -2;
    }
    else if (((*len = lseek(fd, 0, SEEK_END)) == -1) || (lseek(fd, 0, SEEK_SET) == -1))
    {
        ret = -3;
    }
    else if ((*result = calloc(*len + 1, sizeof(char))) == NULL)
    {
        ret = -4;
    }
    else if (read(fd, *result, *len + 1) == -1)
    {
        ret = -5;
    }
    else
    {
        close(fd);
        ret = 0;
    }

    return ret;
}

static void test_get_nb_device_active(void)
{
    int i = 0;
    pb_devices_t* d = NULL;

    struct {
        char* filepath;
        ssize_t result;
    } tests[] = {
        {.filepath = "devices/no_device.json", .result = 0},
        {.filepath = "devices/deactivated.json", .result = 0},
        {.filepath = "devices/one_active.json", .result = 1}
    };

    g_assert_cmpint( pb_devices_get_number_active(d), ==, -1 );


    for (i = 0; i < (sizeof(tests) / sizeof(tests[0])); i++)
    {
        char* json = NULL;
        size_t json_len = 0;

        d = pb_devices_new();
        g_assert_nonnull( d );
        g_assert_cmpint( pb_devices_get_ref(d), ==, 1 );

        printf("%s\n", tests[i].filepath);
        g_assert_cmpint( load_json_from_file(&json, &json_len, tests[i].filepath), ==, 0);
        g_assert_cmpint( pb_devices_load_devices_from_data(d, json, json_len), ==, 0 );

        g_assert_cmpint( pb_devices_get_number_active(d), ==, tests[i].result);

        pb_devices_unref(d);
    }
}

static void test_get_iden_from_name(void)
{
    int i = 0;
    pb_devices_t* d = NULL;

    struct {
        char* filepath;
        char* nickname;
        char* result;
    } tests[] = {
        {.filepath = "devices/no_device.json", .nickname = "Firefox", .result = NULL},
        {.filepath = "devices/deactivated.json", .nickname = "Firefox", .result = NULL},
        {.filepath = "devices/one_active.json", .nickname = "Firefox", .result = "helloworld1"}
    };

    g_assert_cmpint( pb_devices_get_number_active(d), ==, -1 );


    for (i = 0; i < (sizeof(tests) / sizeof(tests[0])); i++)
    {
        char* json = NULL;
        size_t json_len = 0;

        d = pb_devices_new();
        g_assert_nonnull( d );
        g_assert_cmpint( pb_devices_get_ref(d), ==, 1 );

        g_assert_cmpint( load_json_from_file(&json, &json_len, tests[i].filepath), ==, 0);
        g_assert_cmpint( pb_devices_load_devices_from_data(d, json, json_len), ==, 0 );

        g_assert_cmpstr( pb_devices_get_iden_from_name(d, tests[i].nickname), ==, tests[i].result);

        pb_devices_unref(d);
        g_assert_cmpint( pb_devices_get_ref(d), ==, 0 );
    }
}

int main (int argc, char *argv[])
{
    g_test_init (&argc, &argv, NULL);

    g_test_add_func("/devices/empty-devices", test_empty_devices);
    g_test_add_func("/devices/ref-devices", test_ref_devices);

    g_test_add_func("/devices/add-new-device", test_add_new_device);
    g_test_add_func("/devices/load-devices-from-string", test_load_devices_from_string);
    g_test_add_func("/devices/get-number-active", test_get_nb_device_active);
    g_test_add_func("/devices/get-iden-from-name", test_get_iden_from_name);

    return g_test_run ();
}
