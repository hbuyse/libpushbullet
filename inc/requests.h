/**
 * @file requests.h
 * @author hbuyse
 * @date 08/05/2016
 *
 * @brief Contains all the methods in order to communicate to the Pushbullet API
 */


#ifndef __REQUESTS_H__
#define __REQUESTS_H__

#include <urls.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief      GET request for the PushBullet API
 *
 * \param      result       The result buffer
 * \param      url_request  The url request
 * \param      token_key    The user token key
 *
 * \return     HTTP status code
 */
unsigned short pb_get(char *result, char *url_request, char *token_key);


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
unsigned short pb_post(char *result, char *url_request, char *token_key, char *data);


/**
 * \brief      DELETE request for the PushBullet API
 *
 * \param      result       The result buffer
 * \param      url_request  The url request with the data we want to delete (url_encoded)
 * \param      token_key    The user token key
 *
 * \return     HTTP status code
 */
unsigned short pb_delete(char *result, char *url_request, char *token_key);


#ifdef __cplusplus
}
#endif

#endif          // __REQUESTS_H__