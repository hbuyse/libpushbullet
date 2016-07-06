/**
 * @file: main.c
 * @author: hbuyse
 *
 * @brief Testing program
 */
#define _XOPEN_SOURCE
#include <unistd.h>          // getopt, opterr, optarg, optopt, optind
#include <getopt.h>          // struct option
#include <stdlib.h>             // system, exit, EXIT_FAILURE
#include <string.h>          // memset
#include <stdio.h>          // fprintf

#include <test_config.h>          // main_config
#include <test_user.h>          // main_user
#include <test_devices.h>          // main_devices
#include <test_pushes.h>          // main_pushes

int main(int    argc,
         char   *argv[]
         )
{
    int     opt         = 0;
    int     long_index  = 0;


    // char                        cmd[256]; // Buffer for the commands
    unsigned char               res             = 0; // Results of the system calls

    // Token key
    char                        *token_key      = NULL; // Token key pointer


    opterr = 0;

    static struct option        long_options[]  =
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


    fprintf(stdout, "\e[1m############################################################\e[0m\n");
    fprintf(stdout, "\e[1m#                         USER.OUT                         #\e[0m\n");
    fprintf(stdout, "\e[1m############################################################\e[0m\n");
    res |= main_user(token_key);


    fprintf(stdout, "\e[1m############################################################\e[0m\n");
    fprintf(stdout, "\e[1m#                       DEVICES.OUT                        #\e[0m\n");
    fprintf(stdout, "\e[1m############################################################\e[0m\n");
    res |= main_devices(token_key);


    fprintf(stdout, "\e[1m############################################################\e[0m\n");
    fprintf(stdout, "\e[1m#                        CONFIG.OUT                        #\e[0m\n");
    fprintf(stdout, "\e[1m############################################################\e[0m\n");
    res |= main_config(token_key);


    fprintf(stdout, "\e[1m############################################################\e[0m\n");
    fprintf(stdout, "\e[1m#                        PUSHES.OUT                        #\e[0m\n");
    fprintf(stdout, "\e[1m############################################################\e[0m\n");
    // res = main_pushes(token_key);


    return (res);
}