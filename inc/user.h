/**
 * @file user.h
 * @author hbuyse
 * @date 08/05/2016
 *
 * @brief  Functions that have a report with the user informations
 */


#ifndef __USER_H__
#define __USER_H__

#include <pb_structures.h>          // pb_user_t

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief       Download the user's informations from PushBullet using its API
 *
 * @param[out]  user_to_fill  The user structure that we fill
 * @param[in]   token_key     The user's token key
 * @param[in]   config        The JSON configuration
 *
 * @return      HTTP status code
 */
unsigned short pb_get_user_info(pb_user_t *user_to_fill, const char *token_key, const json_object *config);


/**
 * @brief      Free the user structure
 *
 * @param      user  The user we want to free
 */
void pb_free_user(pb_user_t *user);


/**
 * @brief      Gets the configuration as a JSON object.
 *
 * @param[in]  path  Path to the config file
 *
 * @return     The configuration data as a json object.
 */
json_object* pb_get_config_json(const char *path);


/**
 * @brief      Get the HTTP proxy from the configuration
 *
 * @param[in]  user  The user having the configuration
 *
 * @return     HTTP proxy
 */
const char* pb_get_http_proxy(const pb_user_t user);


/**
 * @brief      Get the HTTPS proxy from the configuration
 *
 * @param[in]  user  The user having the configuration
 *
 * @return     HTTPS proxy
 */
const char* pb_get_https_proxy(const pb_user_t user);


/**
 * @brief      Get the CURL timeout from the configuration
 *
 * @param[in]  user  The user having the configuration
 *
 * @return     CURL timeout
 */
int pb_get_curl_timeout(const pb_user_t user);


/**
 * @brief      Get the user token key from the configuration
 *
 * @param[in]  config  The JSON configuration
 *
 * @return     User token key
 */
const char* pb_get_token_key(const json_object *config);




#ifdef __cplusplus
}
#endif

#endif          // __USER_H__