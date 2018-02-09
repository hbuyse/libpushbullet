#include <glib.h>
#include <glib/gi18n.h>

#include <sys/types.h>   // open, lseek
#include <sys/stat.h>   // open
#include <fcntl.h>   // open
#include <unistd.h>   // lseek
#include <stdio.h>

#include "lib/pb_devices_priv.h"
#include "lib/pb_devices_prot.h"
#include "pushbullet.h"


static void test_empty_device(void)
{
    pb_device_t* d = pb_device_new();

    g_assert_nonnull( d );
    g_assert_cmpint( pb_device_get_ref(d), ==, 1 );

    pb_device_unref(d);
    g_assert_cmpint( pb_device_get_ref(d), ==, 0 );
}

static void test_ref_device(void)
{
    pb_device_t* d = NULL;
    g_assert_cmpint( pb_device_ref(d), ==, -1 );
    g_assert_cmpint( pb_device_unref(d), ==, -1 );
 
    d = pb_device_new();

    g_assert_nonnull( d );
    g_assert_cmpint( pb_device_get_ref(d), ==, 1 );
    g_assert_cmpint( pb_device_ref(d), ==, 0 );
    g_assert_cmpint( pb_device_get_ref(d), ==, 2 );
    g_assert_cmpint( pb_device_unref(d), ==, 0 );
    g_assert_cmpint( pb_device_get_ref(d), ==, 1 );
   
    pb_device_unref(d);
    g_assert_cmpint( pb_device_get_ref(d), ==, 0 );
}

int main (int argc, char *argv[])
{
    g_test_init (&argc, &argv, NULL);

    g_test_add_func("/device/empty-device", test_empty_device);
    g_test_add_func("/device/ref-device", test_ref_device);

    return g_test_run ();
}
