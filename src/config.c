#include <stdio.h>          // fprintf, stderr, FILE, fopen, fclose, fseek, ftell, fread
#include <stdlib.h>          // calloc, free, NULL
#include <json/json.h>          // json_object, json_tokener_parse
#include <errno.h>          // %m
#include <string.h>          // strcmp

#include <config.h>          // NUMBER_PROXIES, PROXY_MAX_LENGTH
#include <pb_structures.h>          // pb_user_t

json_object* pb_get_config_json(char *path)
{
    json_object     *config_json;
    FILE            *config_fd          = NULL; // File descriptor to the config file
    size_t          config_file_length  = 0;    // Length of the config file
    char            *config_data        = NULL; // Content of the config file
    size_t          read = 0;   // Read bytes from the config file


    if ( path == NULL )
    {
        fprintf(stderr, "\e[1;31m[%s]\e[0m No path given for the config file.\n", __func__);

        return (NULL);
    }


    // Trying to open the config file
    if ( (config_fd = fopen(path, "r") ) == NULL )
    {
        fprintf(stderr, "\e[1;31m[%s]\e[0m Could not open the config file at %s: %m", __func__, path);

        return (NULL);
    }


    // We get the file of the file
    fseek(config_fd, 0, SEEK_END);
    config_file_length  = ftell(config_fd);
    fseek(config_fd, 0, SEEK_SET);


    // We allocate the memory for the contents
    config_data         = (char *) calloc(config_file_length + 1, sizeof(char) );


    // Read the file and store it in the memory
    read = fread(config_data, sizeof(char), config_file_length, config_fd);

    if ( read != config_file_length )
    {
        fprintf(stderr,
                "\e[1;31m[%s]\e[0m Read %ld on %ld from the config file %s.\n",
                __func__,
                read,
                config_file_length,
                path);
    }


    // Close the file
    fclose(config_fd);


    // Use the JSON parser
    config_json = json_tokener_parse(config_data);


    // Free the memory allocated for the
    if ( config_data )
    {
        free(config_data);
        config_data = NULL;
    }

    return (config_json);
}



unsigned char pb_get_proxies_from_json_config(char  proxies[NUMBER_PROXIES][PROXY_MAX_LENGTH],
                                              char  *path
                                              )
{
    unsigned char       proxies_bits    = 0;
    json_object         *config_json    = NULL;


    config_json = pb_get_config_json(path);

    if ( config_json == NULL )
    {
        return (0);
    }

    json_object_object_foreach(config_json, key, val)
    {
        if ( (strcmp(key, "proxies") == 0) || (strcmp(key, "proxy") == 0) )
        {
            json_object     *proxies_json = val;
            json_object_object_foreach(proxies_json, key2, val2)
            {
                if ( strcmp(key2, "http") == 0 )
                {
                    strcpy(proxies[0], json_object_get_string(val2) );
                }

                if ( strcmp(key2, "https") == 0 )
                {
                    strcpy(proxies[1], json_object_get_string(val2) );
                }
            }
        }
    }


    return (proxies_bits);
}