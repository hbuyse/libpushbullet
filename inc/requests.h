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

#include <pb_structures.h>          // NUMBER_PROXIES, PROXY_MAX_LENGTH


/**
 * \brief      GET request for the PushBullet API
 *
 * \param      result       The result buffer
 * \param      url_request  The url request
 * \param      token_key    The user token key
 *
 * \return     HTTP status code
 */
unsigned short pb_get(char          *result,
                      const char    *url_request,
                      const char    *token_key,
                      const char    proxies[NUMBER_PROXIES][PROXY_MAX_LENGTH]);


/**
 * \brief      POST request for the PushBullet API
 *
 * \param      result       The result buffer
 * \param      url_request  The url request
 * \param      token_key    The user token key
 * \param      data         The data that we send
 *
 * \return     HTTP status code
 */
unsigned short pb_post(char         *result,
                       const char   *url_request,
                       const char   *token_key,
                       const char   *data,
                       const char   proxies[NUMBER_PROXIES][PROXY_MAX_LENGTH]);


/**
 * \brief      DELETE request for the PushBullet API
 *
 * \param      result       The result buffer
 * \param      url_request  The url request with the data we want to delete (url_encoded)
 * \param      token_key    The user token key
 *
 * \return     HTTP status code
 */
unsigned short pb_delete(char       *result,
                         const char *url_request,
                         const char *token_key,
                         const char proxies[NUMBER_PROXIES][PROXY_MAX_LENGTH]);


#ifdef __cplusplus
}
#endif

#endif          // __REQUESTS_H__