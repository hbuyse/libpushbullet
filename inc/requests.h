/**
 * @file requests.h
 * @date 08/05/2016
 *
 * @brief Contains all the methods in order to communicate to the Pushbullet API
 */


#ifndef __REQUESTS_H__
#define __REQUESTS_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief Version of the library
 */
#define VERSION "0.0.1"


/**
 * \brief Basic URL for PushBullet API
 */
#define API_URL "https://api.pushbullet.com/v2/"


/**
 * \brief PushBullet API URL for the pushes
 */
#define API_URL_PUSHES API_URL "pushes"


/**
 * \brief PushBullet API URL for the devices
 */
#define API_URL_DEVICES API_URL "devices"


/**
 * \brief PushBullet API URL for the user informations
 */
#define API_URL_ME API_URL "users/me"


/**
 * \brief PushBullet API URL to get contacts from the user account
 */
#define API_URL_CONTACTS API_URL "contacts"


/**
 * \brief PushBullet API URL to do upload requests
 */
#define API_URL_FILE_REQUEST API_URL "upload-request"


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
unsigned short pb_delete(char   *result, char   *url_request, char   *token_key);


#ifdef __cplusplus
}
#endif

#endif          // __REQUESTS_H__