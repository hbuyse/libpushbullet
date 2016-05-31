#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <json/json.h>          // json_object
#include <pb_structures.h>          // pb_user_t


/**
 * \brief      Gets the configuration as a JSON object.
 *
 * \param[in]  path  Path to the config file
 *
 * \return     The configuration data as a json object.
 */
json_object* pb_get_config_json(char *path);


/**
 * \brief      Gets the proxies from the JSON configuration file.
 *
 * \param      user     The user where we store the proxies informations
 * \param[in]  path     The path to the configuration file
 *
 * \return     The number of proxies from json configuration.
 */
unsigned char     pb_get_proxies_from_json_config(char proxies[NUMBER_PROXIES][PROXY_MAX_LENGTH], char *path);


#endif          // __CONFIG_H_