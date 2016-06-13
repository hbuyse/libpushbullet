/**
 * @file: main.c
 * @author: hbuyse
 *
 * @brief Testing program for user functions
 */

#include <unistd.h>          // getopt, opterr, optarg, optopt, optind
#include <getopt.h>          // struct option
#include <stdlib.h>             // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>          // strcmp
#include <stdio.h>          // fprintf

#include <pb_structures.h>          // pb_user_t, pb_device_t, pb_browser_t, pb_phone_t, ICON_PHONE, ICON_BROWSER
#include <user.h>          // pb_get_user_info, pb_free_user
#include <devices.h>          // pb_get_devices
#include <http_code.h>          // HTTP_OK
#include <pushes.h>          // pb_push_note, pb_push_link

#include "../minunit/minunit.h"



/**
 * \brief Stores the token key given as option
 */
static char                 *token_key  = NULL;
static pb_user_t            user;
static unsigned short       res         = 0;


/**
 * \brief      Setup function for devices related functions
 */
static void setup_devices(void)
{
    memset(&user, 0, sizeof(pb_user_t) );

    res = pb_get_user_info(&user, token_key, NULL);
    mu_check(res == HTTP_OK);
}



/**
 * \brief      Setup function get all the informations
 */
static void setup(void)
{
    memset(&user, 0, sizeof(pb_user_t) );

    res = pb_get_user_info(&user, token_key, NULL);
    mu_check(res == HTTP_OK);

    res = pb_get_devices(&user);
    mu_check(res == HTTP_OK);
}



/**
 * \brief      Teardown function: free the devices and the user
 */
static void teardown(void)
{
    pb_free_user(&user);
}



MU_TEST(test_get_devices)
{
    pb_device_t     *tmp        = NULL;
    pb_device_t     *devices    = NULL;


    // Test with no token_key.
    // It should return a 401 Unauthorized
    user.token_key  = NULL;
    res = pb_get_devices(&user);

    mu_check(res == HTTP_UNAUTHORIZED);


    // Get the devices
    user.token_key  = token_key;
    res = pb_get_devices(&user);


    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);
    mu_check(user.devices != NULL);

    devices         = user.devices;

    for ( tmp = devices; tmp != NULL; tmp = tmp->next )
    {
        switch ( tmp->type_device )
        {
            case ICON_PHONE:
                mu_check(tmp->phone.active == 1);
                mu_check(strcmp(tmp->phone.icon, "phone") == 0);
                break;

            case ICON_BROWSER:
                mu_check(tmp->browser.active == 1);
                mu_check(strcmp(tmp->browser.icon, "browser") == 0);
                break;

            default:
                mu_fail("type_device: unknown\n");
        }
    }

    pb_free_devices(&user);
    mu_check(user.devices == NULL);
}


MU_TEST(test_free_devices)
{
    unsigned char       res = 0;
    pb_user_t           user;


    // All datas to zero
    memset(&user, 0, sizeof(pb_user_t) );


    // Get the user
    res = pb_get_user_info(&user, token_key, NULL);


    // Check the results after downloading the user informations
    mu_check(res == HTTP_OK);


    // Get the devices
    res = pb_get_devices(&user);


    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);
    mu_check(user.devices != NULL);


    // Free the list of devices
    pb_free_devices(&user);

    mu_check(user.devices == NULL);
}


MU_TEST(test_get_number_active_devices)
{
    pb_user_t tmp;

    memset(&tmp, 0, sizeof(pb_user_t));

    mu_check(tmp.devices == NULL);
    mu_check(pb_get_number_active_devices(tmp) == 0);


    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);
    mu_check(user.devices != NULL);
    mu_check(pb_get_number_active_devices(user) != 0);
}


MU_TEST(test_get_iden_from_name)
{
    pb_user_t tmp;

    memset(&tmp, 0, sizeof(pb_user_t));

    mu_check(tmp.devices == NULL);

    mu_check(pb_get_iden_from_name(tmp, NULL) == NULL);
    mu_check(pb_get_iden_from_name(tmp, "null") == NULL);
    mu_check(pb_get_iden_from_name(tmp, "Chrome") != NULL);


    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);
    mu_check(user.devices != NULL);


    // Check the function pb_get_iden_from_name
    mu_check(pb_get_iden_from_name(user, NULL) == NULL);
    mu_check(pb_get_iden_from_name(user, "null") == NULL);
    mu_check(pb_get_iden_from_name(user, "Chrome") != NULL);
}


MU_TEST_SUITE(test_devices)
{
    MU_SUITE_CONFIGURE(&setup_devices, NULL);


    // Test
    fprintf(stdout, "\n\e[1m[test_get_devices]\e[0m\t");
    MU_RUN_TEST(test_get_devices);
    fprintf(stdout, "\n\e[1m[test_free_devices]\e[0m\t");
    MU_RUN_TEST(test_free_devices);
}


MU_TEST_SUITE(test_on_devices_info)
{
    MU_SUITE_CONFIGURE(&setup, &teardown);


    // Test
    fprintf(stdout, "\n\e[1m[test_get_number_active_devices]\e[0m\t");
    MU_RUN_TEST(test_get_number_active_devices);
    fprintf(stdout, "\n\e[1m[test_get_iden_from_name]\e[0m\t");
    MU_RUN_TEST(test_get_iden_from_name);
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


    // Launch the test suites
    MU_RUN_SUITE(test_devices);
    MU_RUN_SUITE(test_on_devices_info);


    // Print the report
    MU_REPORT();

    return ( (minunit_fail == 0) ? 0 : 1);
}