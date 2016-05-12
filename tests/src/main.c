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
#include <pushes.h>          // pb_push_note, pb_push_link

#include "../minunit/minunit.h"


/**
 * \brief Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000


/**
 * \brief Stores the token key given as option
 */
static char                 *token_key  = NULL;
static PB_user_t            user;
static unsigned short       res         = 0;


/**
 * \brief      Setup function for user related functions
 */
void setup_user(void)
{
    memset(&user, 0, sizeof(PB_user_t) );
}



/**
 * \brief      Setup function for devices related functions
 */
void setup_devices(void)
{
    memset(&user, 0, sizeof(PB_user_t) );

    res = pb_get_user_info(&user, token_key);
    mu_check(res == HTTP_OK);
}



/**
 * \brief      Setup function get all the informations
 */
void setup(void)
{
    memset(&user, 0, sizeof(PB_user_t) );

    res = pb_get_user_info(&user, token_key);
    mu_check(res == HTTP_OK);

    res = pb_get_devices(&user);
    mu_check(res == HTTP_OK);
}



/**
 * \brief      Teardown function: free the devices and the user
 */
void teardown(void)
{
    pb_free_user(&user);
}



MU_TEST(test_get_user_info)
{
    res = pb_get_user_info(&user, token_key);

    mu_check(res == HTTP_OK);

    mu_check(user.token_key != NULL);
    mu_check(user.active == 1);
    mu_check(user.created != 0);
    mu_check(user.modified != 0);
    mu_check(user.name != NULL);
    mu_check(user.iden != NULL);
    mu_check(user.image_url != NULL);
    mu_check(user.email != NULL);
    mu_check(user.email_normalized != NULL);
    mu_check(user.max_upload_size == 26214400);
}


MU_TEST(test_free_user)
{
    unsigned char       res = 0;
    PB_user_t           user;


    // All datas to zero
    memset(&user, 0, sizeof(PB_user_t) );

    res = pb_get_user_info(&user, token_key);

    mu_check(res == HTTP_OK);

    pb_free_user(&user);

    mu_check(user.token_key == 0);
    mu_check(user.active == 0);
    mu_check(user.created == 0);
    mu_check(user.modified == 0);
    mu_check(user.name == NULL);
    mu_check(user.iden == NULL);
    mu_check(user.image_url == NULL);
    mu_check(user.email == NULL);
    mu_check(user.email_normalized == NULL);
    mu_check(user.max_upload_size == 0);
}


MU_TEST(test_get_devices)
{
    PB_device_t     *tmp        = NULL;
    PB_device_t     *devices    = NULL;


    // Get the devices
    res     = pb_get_devices(&user);


    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);
    mu_check(user.devices != NULL);

    devices = user.devices;

    for ( tmp = devices; tmp != NULL; tmp = tmp->next )
    {
        switch ( tmp->type_device )
        {
            case TYPE_ANDROID:
            case TYPE_IPHONE:
                mu_check(tmp->phone.active == 1);
                mu_check(strcmp(tmp->phone.icon, "phone") == 0);
                break;

            case TYPE_CHROME:
            case TYPE_FIREFOX:
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
    PB_user_t           user;


    // All datas to zero
    memset(&user, 0, sizeof(PB_user_t) );


    // Get the user
    res = pb_get_user_info(&user, token_key);


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
    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);
    mu_check(user.devices != NULL);
    mu_check(pb_get_number_active_devices(user) != 0);
}


MU_TEST(test_get_iden_from_name)
{
    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);
    mu_check(user.devices != NULL);


    // Check the function pb_get_iden_from_name
    mu_check(pb_get_iden_from_name(user, NULL) == NULL);
    mu_check(pb_get_iden_from_name(user, "null") == NULL);
    mu_check(pb_get_iden_from_name(user, "Chrome") != NULL);
}


MU_TEST(test_push_note)
{
    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);


    // Check the function pb_push_note
    char     *result = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    mu_check(pb_push_note(result, "Test", "test", NULL, user) == HTTP_OK);
}


MU_TEST(test_push_link)
{
    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);


    // Check the function pb_push_link
    char     *result = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    mu_check(pb_push_link(result, "Test", "test", "http://www.google.fr", NULL, user) == HTTP_OK);
}


MU_TEST_SUITE(test_user)
{
    MU_SUITE_CONFIGURE(&setup_user, NULL);


    // Test

    fprintf(stdout, "\n\e[1m[test_get_user_info]\e[0m\t");
    MU_RUN_TEST(test_get_user_info);
    fprintf(stdout, "\n\e[1m[test_free_user]\e[0m\t");
    MU_RUN_TEST(test_free_user);
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


MU_TEST_SUITE(test_pushes)
{
    MU_SUITE_CONFIGURE(&setup, &teardown);


    // Test
    fprintf(stdout, "\n\e[1m[test_push_note]\e[0m\t");
    MU_RUN_TEST(test_push_note);
    fprintf(stdout, "\n\e[1m[test_push_link]\e[0m\t");
    MU_RUN_TEST(test_push_link);
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
    MU_RUN_SUITE(test_user);
    MU_RUN_SUITE(test_devices);
    MU_RUN_SUITE(test_on_devices_info);
    MU_RUN_SUITE(test_pushes);


    // Print the report
    MU_REPORT();

    return ( (minunit_fail == 0) ? 0 : 1);
}