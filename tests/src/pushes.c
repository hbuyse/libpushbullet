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
 * @brief Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000


/**
 * \brief Stores the token key given as option
 */
static char                 *token_key  = NULL;
static pb_user_t            user;
static unsigned char        res         = 0;



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



MU_TEST(test_push_note)
{
    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);


    // Check the function pb_push_note
    char            *result = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );


    // Send an empty note
    pb_note_t       note0   =
    {
        .title  = NULL,
        .body   = NULL
    };
    mu_check(pb_push_note(result, note0, NULL, user) == HTTP_OK);


    // Send a note with no title
    pb_note_t     note1     =
    {
        .title  = NULL,
        .body   = "Empty title"
    };
    mu_check(pb_push_note(result, note1, NULL, user) == HTTP_OK);


    // Send a note with an empty body
    pb_note_t     note2     =
    {
        .title  = "Empty body",
        .body   = NULL
    };
    mu_check(pb_push_note(result, note2, NULL, user) == HTTP_OK);


    // Send a good note
    pb_note_t     note3     =
    {
        .title  = "Test",
        .body   = "test"
    };
    mu_check(pb_push_note(result, note3, NULL, user) == HTTP_OK);

    if ( result )
    {
        free(result);
    }
}


MU_TEST(test_push_link)
{
    // Check the results after downloading the devices informations
    mu_check(res == HTTP_OK);


    // Check the function pb_push_link
    char            *result = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );


    // Send an empty link
    pb_link_t       link0   =
    {
        .title  = NULL,
        .body   = NULL,
        .url    = NULL
    };
    mu_check(pb_push_link(result, link0, NULL, user) == HTTP_OK);


    // Send a link with a title only
    pb_link_t     link1     =
    {
        .title  = "Only title",
        .body   = NULL,
        .url    = NULL
    };
    mu_check(pb_push_link(result, link1, NULL, user) == HTTP_OK);


    // Send a link with a body only
    pb_link_t     link2     =
    {
        .title  = NULL,
        .body   = "Only body",
        .url    = NULL
    };
    mu_check(pb_push_link(result, link2, NULL, user) == HTTP_OK);


    // Send a link with an url only
    pb_link_t     link3     =
    {
        .title  = NULL,
        .body   = NULL,
        .url    = "https://www.google.com"
    };
    mu_check(pb_push_link(result, link3, NULL, user) == HTTP_OK);


    // Send a link witout title
    pb_link_t     link4     =
    {
        .title  = NULL,
        .body   = "Body",
        .url    = "https://www.google.com"
    };
    mu_check(pb_push_link(result, link4, NULL, user) == HTTP_OK);


    // Send a link witout body
    pb_link_t     link5     =
    {
        .title  = "Title",
        .body   = NULL,
        .url    = "https://www.google.com"
    };
    mu_check(pb_push_link(result, link5, NULL, user) == HTTP_OK);


    // Send a link witout body
    pb_link_t     link6     =
    {
        .title  = "Title",
        .body   = "Body",
        .url    = NULL
    };
    mu_check(pb_push_link(result, link6, NULL, user) == HTTP_OK);


    // Send a link witout body
    pb_link_t     link7     =
    {
        .title  = "Title",
        .body   = "Body",
        .url    = "https://www.google.com"
    };
    mu_check(pb_push_link(result, link7, NULL, user) == HTTP_OK);

    if ( result )
    {
        free(result);
    }
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
    MU_RUN_SUITE(test_pushes);


    // Print the report
    MU_REPORT();

    return ( (minunit_fail == 0) ? 0 : 1);
}