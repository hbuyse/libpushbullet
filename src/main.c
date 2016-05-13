/**
 * @file: main.c
 * @author: hbuyse
 */

#include <stdio.h>          // fprintf, stdout, stderr
#include <stdlib.h>          // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>          // getopt, opterr, optarg, optopt, optind
#include <getopt.h>          // struct option

#include <user.h>          // pb_get_user_info
#include <devices.h>          // pb_get_devices
#include <http_code.h>          // HTTP_OK
#include <pushes.h>          // pb_push_note


/**
 * \brief Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000


/**
 * @brief Shows how to use the program
 *
 * @param program_name The program's name
 */
static void usage(char *program_name)
{
    fprintf(stdout, "Usage: %s [-h] -t token_key \n", program_name);
    fprintf(stdout, "\t-h | --help         Display this help.\n");
    fprintf(stdout, "\t-t | --token-key    Pushbullet token key\n");
}



/**
 * @brief The program
 *
 * @param argc Number of arguments
 * @param argv Lists of pointers that points to the arguments
 *
 * @return Exit code
 */
int main(int    argc,
         char   **argv
         )
{
    int                         opt             = 0;
    int                         long_index      = 0;
    char                        *token_key      = NULL;

    pb_user_t                   user;
    unsigned char               res             = 0;


    opterr = 0;

    static struct option        long_options[]  =
    {
        {"help", no_argument, 0, 'h'},
        {"token-key", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    if ( argc == 1 )
    {
        usage(argv[0]);
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


            case 'h':
            {
                usage(argv[0]);
                exit(EXIT_SUCCESS);
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

                usage(argv[0]);
                exit(EXIT_FAILURE);
            }

            default:
            {
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    res = pb_get_user_info(&user, token_key);
    res = pb_get_devices(&user);

    char     *result = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    pb_push_note(result, "Hello", "Hello World", "LGE Nexus 5X", user);


    // pb_push_link("Google", "Hello World", "http://www.google.fr", pb_get_iden_from_name(user, "LGE Nexus 5X"), user);

    pb_free_user(&user);

    return (0);
}