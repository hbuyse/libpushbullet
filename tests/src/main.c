/**
 * @file: main.c
 * @author: hbuyse
 *
 * @brief Testing program
 */

#include <unistd.h>          // getopt, opterr, optarg, optopt, optind
#include <getopt.h>          // struct option
#include <stdlib.h>             // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>          // strcmp
#include <stdio.h>          // fprintf

#include <user.h>          // PB_user_t, pb_get_user_info, pb_free_user
#include <devices.h>          // pb_get_devices
#include <http_code.h>          // HTTP_OK

#include "../minunit/minunit.h"

static char     *token_key = NULL;


MU_TEST(test_get_user_info)
{
    unsigned char       res = 0;
    PB_user_t           user;


    // All datas to zero
    memset(&user, 0, sizeof(PB_user_t));

    fprintf(stdout, "\e[1m[%s]\e[0m\t", __func__);

    res = pb_get_user_info(&user, token_key);

    mu_assert(res == HTTP_OK, "res should be HTTP_OK.");
    mu_assert(user.token_key != NULL, "user.token_key should not be NULL.");
    mu_assert(user.active == 1, "user.active should be 1.");
    mu_assert(user.created != 0, "user.created should not be 0.");
    mu_assert(user.modified != 0, "user.modified should not be 0.");
    mu_assert(user.name != NULL, "user.name should not be NULL");
    mu_assert(user.iden != NULL, "user.iden should not be NULL");
    mu_assert(user.image_url != NULL, "user.image_url should not be NULL");
    mu_assert(user.email != NULL, "user.email should not be NULL");
    mu_assert(user.email_normalized != NULL, "user.email_normalized should not be NULL");
    mu_assert(user.max_upload_size == 26214400, "user.max_upload_size should be 26214400");

    fprintf(stdout, "\n");
}


MU_TEST(test_get_devices)
{
    PB_device_t         *tmp        = NULL;
    PB_device_t         *devices    = NULL;
    unsigned char       res         = 0;
    PB_user_t           user        =
    {
        .token_key = token_key
    };


    res     = pb_get_devices(&user);

    fprintf(stdout, "\e[1m[%s]\e[0m\t", __func__);

    mu_assert(res == HTTP_OK, "res should be HTTP_OK.");
    mu_assert(user.devices != NULL, "user.devices should not be NULL");

    devices = user.devices;

    for ( tmp = devices; tmp != NULL; tmp = tmp->next )
    {
        switch ( tmp->type_device )
        {
            case TYPE_ANDROID:
            case TYPE_IPHONE:
                mu_assert(tmp->phone.active == 1, "Phone should be active");
                mu_assert(strcmp(tmp->phone.icon, "phone") == 0, "Phone icon should be \"phone\"");
                break;

            case TYPE_CHROME:
            case TYPE_FIREFOX:
                mu_assert(tmp->browser.active == 1, "Browser should be active");
                mu_assert(strcmp(tmp->browser.icon, "browser") == 0, "Browser icon should be \"browser\"");
                break;

            default:
                mu_fail("type_device: unknown\n");
        }
    }

    pb_free_devices(&user);
    mu_assert(user.devices == NULL, "user.devices should be NULL");

    fprintf(stdout, "\n");
}


MU_TEST(test_free_devices)
{
    unsigned char       res = 0;
    PB_user_t           user;


    // All datas to zero
    memset(&user, 0, sizeof(PB_user_t));

    fprintf(stdout, "\e[1m[%s]\e[0m\t", __func__);

    // Get the user
    res = pb_get_user_info(&user, token_key);
    // Check the results after downloading the user informations
    mu_assert(res == HTTP_OK, "res should be HTTP_OK.");

    // Get the devices
    res = pb_get_devices(&user);

    // Check the results after downloading the devices informations
    mu_assert(res == HTTP_OK, "res should be HTTP_OK.");
    mu_assert(user.devices != NULL, "user.modified should not be 0.");

    // Free the list of devices
    pb_free_devices(&user);

    mu_assert(user.devices == NULL, "user.devices should not NULL");

    fprintf(stdout, "\n");
}

MU_TEST(test_free_user)
{
    unsigned char       res = 0;
    PB_user_t           user;


    // All datas to zero
    memset(&user, 0, sizeof(PB_user_t));

    fprintf(stdout, "\e[1m[%s]\e[0m\t", __func__);

    res = pb_get_user_info(&user, token_key);

    mu_assert(res == HTTP_OK, "res should be HTTP_OK.");

    pb_free_user(&user);

    mu_assert(user.token_key == 0, "user.active should be NULL.");
    mu_assert(user.active == 0, "user.active should be 0.");
    mu_assert(user.created == 0, "user.created should be 0.");
    mu_assert(user.modified == 0, "user.modified should be 0.");
    mu_assert(user.name == NULL, "user.name should be NULL");
    mu_assert(user.iden == NULL, "user.iden should be NULL");
    mu_assert(user.image_url == NULL, "user.image_url should be NULL");
    mu_assert(user.email == NULL, "user.email should be NULL");
    mu_assert(user.email_normalized == NULL, "user.email_normalized should be NULL");
    mu_assert(user.max_upload_size == 0, "user.max_upload_size should be 0");

    fprintf(stdout, "\n");
}


MU_TEST(test_get_number_active_devices)
{
    unsigned char       res = 0;
    PB_user_t           user;


    // All datas to zero
    memset(&user, 0, sizeof(PB_user_t));

    fprintf(stdout, "\e[1m[%s]\e[0m\t", __func__);

    // Get the user
    res = pb_get_user_info(&user, token_key);
    // Check the results after downloading the user informations
    mu_assert(res == HTTP_OK, "res should be HTTP_OK.");

    // Get the devices
    res = pb_get_devices(&user);

    // Check the results after downloading the devices informations
    mu_assert(res == HTTP_OK, "res should be HTTP_OK.");
    mu_assert(user.devices != NULL, "user.devices should not be NULL");
    mu_assert(pb_get_number_active_devices(user) != 0, "pb_get_number_active_devices should not be 0");

    pb_free_user(&user);
    fprintf(stdout, "\n");
}


MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(test_get_user_info);
    MU_RUN_TEST(test_get_devices);
    MU_RUN_TEST(test_free_devices);
    MU_RUN_TEST(test_free_user);
    MU_RUN_TEST(test_get_number_active_devices);
}


int main(int    argc,
         char   *argv[]
         )
{
    int     opt         = 0;
    int     long_index  = 0;


    opterr = 0;

    static struct option     long_options[] =
    {
        {"token-key", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    if ( argc == 1 )
    {
        fprintf(stderr, "Usage: %s [-h] -t token_key \n", argv[0]);
        exit(EXIT_FAILURE);
    }


    /* CAREFUL:
     * An option character in this string can be followed by a colon (‘:’) to indicate that it takes a required
     * argument. If an option character is followed by two colons (‘::’), its argument is optional; this is a GNU
     * extension.
     */
    while ( (opt = getopt_long(argc, argv, "ht:", long_options, &long_index) ) != -1 )
    {
        switch ( opt )
        {
            case 't':
            {
                token_key = optarg;
                break;
            }

            case '?':
            {
                if ( optopt == 't' )
                {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                else
                {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                }

                exit(EXIT_FAILURE);
            }

            default:
            {
                exit(EXIT_FAILURE);
            }
        }
    }

    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    return ((minunit_fail == 0) ? 0 : 1);
}