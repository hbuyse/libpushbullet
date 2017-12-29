/**
 * @file: main.c
 * @author: hbuyse
 */

#include <stdio.h>          // fprintf, stdout, stderr
#include <stdlib.h>          // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>             // memset
#include <unistd.h>          // getopt, opterr, optarg, optopt, optind
#include <getopt.h>          // struct option

#include "pushbullet.h"          // pb_get_user_info


/**
 * @brief Maximum length of the buffer (4ko - 4096 - 0x1000)
 */
#define     BUF_MAX_LENGTH 0x1000


/**
 * @brief Shows how to use the program
 *
 * @param program_name The program's name
 */
static void usage(char *program_name)
{
    fprintf(stdout, "Usage: %s [-h] -t token_key -c config_file_path\n", program_name);
    fprintf(stdout, "\t-h | --help              Display this help.\n");
    fprintf(stdout, "\t-t | --token=API_KEY     Pushbullet token key\n");
    fprintf(stdout, "\t-c | --config=FILE       Pushbullet configuration file\n");
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
    int                 opt                 = 0;
    int                 long_index          = 0;
    pb_config_t         *p_config = pb_config_new();

    pb_user_t           *user = pb_user_new();

    opterr = 0;

    static struct option     long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"token", required_argument, 0, 't'},
        {"config", required_argument, 0, 'c'},
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
    while ( (opt = getopt_long(argc, argv, "ht:c:", long_options, &long_index) ) != -1 )
    {
        switch ( opt )
        {
            case 't':
                {
                    pb_config_set_token_key(p_config, optarg);
                    break;
                }

            case 'c':
                {
                    pb_config_from_json_file(p_config, optarg);
                    break;
                }


            case 'h':
                {
                    usage(argv[0]);
                    exit(EXIT_SUCCESS);
                }

            case '?':
                {
                    if ( (optopt == 't') || (optopt == 'c') )
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


    pb_user_set_config(user, p_config);

    pb_user_get_info(user);


    // res = pb_get_devices(&user);


    // result  = (char *) calloc(BUF_MAX_LENGTH, sizeof(char) );

    // pb_file_t     file =
    // {
    //     .file_path  = "tests/volley.png",
    //     .title      = "Volleyball",
    //     .body       = "Volleyball N&B"
    // };

    // pb_push_file(result, &file, NULL, user);

    pb_user_free(user);

    // if ( result )
    // {
    //     free(result);
    //     result = NULL;
    // }

    return (0);
}