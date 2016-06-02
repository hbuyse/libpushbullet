/**
 * @file requests.h
 * @author hbuyse
 * @date 08/05/2016
 *
 * @brief  Functions in order to communicate to the Pushbullet API
 */


#ifndef __REQUESTS_H__
#define __REQUESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <pb_structures.h>          // pb_user_t, NUMBER_PROXIES, PROXY_MAX_LENGTH


/**
 * @brief      GET request for the PushBullet API
 *
 * @param      result       The result buffer
 * @param      url_request  The url request
 * @param      user         The user informations
 *
 * @return     HTTP status code
 */
unsigned short pb_get(char *result, const char *url_request, const pb_user_t user);


/**
 * @brief      POST request for the PushBullet API
 *
 * @param      result       The result buffer
 * @param      url_request  The url request
 * @param      user         The user informations
 * @param      data         The data that we send
 *
 * @return     HTTP status code
 */
unsigned short pb_post(char *result, const char *url_request, const pb_user_t user, const char *data);


/**
 * @brief      DELETE request for the PushBullet API
 *
 * @param      result       The result buffer
 * @param      url_request  The url request with the data we want to delete (url_encoded)
 * @param      user         The user informations
 *
 * @return     HTTP status code
 */
unsigned short pb_delete(char *result, const char *url_request, const pb_user_t user);


#ifdef __cplusplus
}
#endif

#endif          // __REQUESTS_H__