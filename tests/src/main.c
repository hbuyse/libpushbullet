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

#include <user.h>          // PB_user_t, pb_get_user_info
#include <devices.h>          // pb_get_devices
#include <http_code.h>          // HTTP_OK

#include "../minunit/minunit.h"

static char     *token_key = NULL;


MU_TEST(test_user)
{
    unsigned char       res = 0;
    PB_user_t           user;


    res = pb_get_user_info(&user, token_key);

    fprintf(stdout, "\e[1m[%s]\e[0m\t", __func__);

    mu_assert(res == HTTP_OK, "res should be HTTP_OK.");
    mu_assert(user.active == 1, "user.active should be 1.");
    mu_assert(strcmp(user.name, "Henri Buyse") == 0, "user.name should be \"Henri Buyse\"");
    mu_assert(strcmp(user.iden, "ujEIL5AaxhY") == 0, "user.iden should be \"ujEIL5AaxhY\"");
    mu_assert(strcmp(user.email, "henri.buyse@gmail.com") == 0, "user.email should be \"henri.buyse@gmail.com\"");
    mu_assert(strcmp(user.email_normalized, "henribuyse@gmail.com") == 0,
              "user.email_normalized should be \"henribuyse@gmail.com\"");

    fprintf(stdout, "\n");
}


MU_TEST(test_devices)
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

    fprintf(stdout, "\n");
}


MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(test_user);
    MU_RUN_TEST(test_devices);
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

    return (0);
}