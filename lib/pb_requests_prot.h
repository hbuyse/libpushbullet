/**
 * @file pb_requests_prot.h
 * @author hbuyse
 * @date 30/01/2018
 *
 * @brief  Functions in order to communicate to the Pushbullet API
 */


#ifndef __PB_REQUESTS_PROT_H__
#define __PB_REQUESTS_PROT_H__


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Basic URL for PushBullet API
 */
#define API_URL                 "https://api.pushbullet.com/v2/"


/**
 * @brief PushBullet API URL for the pushes
 */
#define API_URL_PUSHES          API_URL "pushes"


/**
 * @brief PushBullet API URL for the devices
 */
#define API_URL_DEVICES         API_URL "devices"


/**
 * @brief PushBullet API URL for the user informations
 */
#define API_URL_ME              API_URL "users/me"
// #define API_URL_ME              "file://user.me"


/**
 * @brief PushBullet API URL to get contacts from the user account
 */
#define API_URL_CONTACTS        API_URL "contacts"


/**
 * @brief PushBullet API URL to do upload requests
 */
#define API_URL_FILE_REQUEST    API_URL "upload-request"


/**
 * @typedef pb_file_t
 * @brief Type definition of the structure pb_file_s
 */
typedef struct pb_file_s pb_file_t;

/**
 * @ingroup  pb_config
 * @typedef  pb_config_t
 * @struct   pb_config_s
 * @brief    Opaque structure containing the value of the configuration
 */
typedef struct pb_config_s  pb_config_t;

/**
 * @brief HTTP codes definition
 */
typedef enum http_code_e http_code_t;

/**
 * @brief      GET request for the PushBullet API
 *
 * @param[out] result       The result buffer
 * @param[in]  url_request  The url request
 * @param[in]  config       The user informations
 *
 * @return     HTTP status code
 */
http_code_t pb_requests_get(char **result, size_t* length, const char *url_request, const pb_config_t* p_config);


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
http_code_t pb_requests_post(char *result, size_t* length, const char *url_request, const pb_config_t* p_config, const char *data);


/**
 * @brief      POST request for the PushBullet API
 *
 * @param      result       The result buffer
 * @param      url_request  The url request
 * @param      user         The user informations
 * @param      file         The file informations
 *
 * @return     HTTP status code
 */
http_code_t pb_requests_post_multipart(char *result, size_t *length,  const char *url_request, const pb_config_t *p_config, const pb_file_t *file);


/**
 * @brief      DELETE request for the PushBullet API
 *
 * @param      result       The result buffer
 * @param      url_request  The url request with the data we want to delete (url_encoded)
 * @param      user         The user informations
 *
 * @return     HTTP status code
 */
http_code_t pb_requests_delete(char *result, size_t* length, const char *url_request, const pb_config_t* p_config);



#ifdef __cplusplus
}
#endif

#endif          // __PB_REQUESTS_PROT_H__
