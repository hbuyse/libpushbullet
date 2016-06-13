/**
 * @file urls.h
 * @author hbuyse
 * @date 12/05/2016
 *
 * @brief  URLs that are needed for the libpushbullet
 */


#ifndef __URLS_H__
#define __URLS_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Basic URL for PushBullet API
 */
#define API_URL "https://api.pushbullet.com/v2/"


/**
 * @brief PushBullet API URL for the pushes
 */
#define API_URL_PUSHES API_URL "pushes"


/**
 * @brief PushBullet API URL for the devices
 */
#define API_URL_DEVICES API_URL "devices"


/**
 * @brief PushBullet API URL for the user informations
 */
#define API_URL_ME API_URL "users/me"


/**
 * @brief PushBullet API URL to get contacts from the user account
 */
#define API_URL_CONTACTS API_URL "contacts"


/**
 * @brief PushBullet API URL to do upload requests
 */
#define API_URL_FILE_REQUEST API_URL "upload-request"


#ifdef __cplusplus
}
#endif

#endif          // __URLS_H__