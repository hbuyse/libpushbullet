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
static char     *token_key = NULL;


/**
 * \brief      Fill user structures with static informations
 *
 * \param[in]  user       The user
 * \param[in]  token_key  The user's token key
 */
static void fill_user_values(pb_user_t  *user,
                             const char *token_key
                             )
{
    // File user with values
    user->active            = 1;
    user->created           = 12;
    user->modified          = 154;
    user->max_upload_size   = 3521;

    user->token_key         = (char *) calloc(128, sizeof(char) );
    user->name = (char *) calloc(128, sizeof(char) );
    user->iden = (char *) calloc(128, sizeof(char) );
    user->image_url         = (char *) calloc(128, sizeof(char) );
    user->email             = (char *) calloc(128, sizeof(char) );
    user->email_normalized  = (char *) calloc(128, sizeof(char) );

    strncpy( (char *) user->token_key, token_key, strlen(token_key) );
    strncpy( (char *) user->name, "Toto Tata", 9);
    strncpy( (char *) user->iden, "hello1254123", 12);
    strncpy( (char *) user->image_url, "https://www.google.com", 22);
    strncpy( (char *) user->email, "toto.tata@tototata.com", 22);
    strncpy( (char *) user->email_normalized, "tototata@tototata.com", 21);
}



MU_TEST(test_get_user_info)
{
    pb_user_t           user;
    unsigned short      res = 0;


    // Nullify the structure
    memset(&user, 0, sizeof(pb_user_t) );


    // No token_key
    res = pb_get_user_info(&user, NULL, NULL);

    mu_check(res == HTTP_UNAUTHORIZED);

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


    // With token key
    res = pb_get_user_info(&user, token_key, NULL);

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
    pb_user_t     user;


    // All datas to zero
    memset(&user, 0, sizeof(pb_user_t) );

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


    // File user with values
    fill_user_values(&user, token_key);

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


MU_TEST_SUITE(test_user)
{
    // Test
    fprintf(stdout, "\n\e[1m[test_get_user_info]\e[0m\t");
    MU_RUN_TEST(test_get_user_info);
    fprintf(stdout, "\n\e[1m[test_free_user]\e[0m\t");
    MU_RUN_TEST(test_free_user);
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


    // Print the report
    MU_REPORT();

    return ( (minunit_fail == 0) ? 0 : 1);
}