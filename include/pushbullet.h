/**
 * @file pushbullet.h
 * @author hbuyse
 * @date 28/12/2017
 *
 * @brief  Pushbullet public header
 */

#ifndef __PUSHBULLET_H__
#define __PUSHBULLET_H__

#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define UNUSED __attribute__((unused))
#define PB_EXPORT extern

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @typedef pb_note_t
 * @brief Type definition of the structure pb_note_s
 */
typedef struct pb_note_s pb_note_t;


/**
 * @typedef pb_link_t
 * @brief Type definition of the structure pb_link_s
 */
typedef struct pb_link_s pb_link_t;


/**
 * @typedef pb_file_t
 * @brief Type definition of the structure pb_file_s
 */
typedef struct pb_file_s pb_file_t;


/**
 * @typedef pb_push_t
 * @brief Type definition of the structure pb_push_s
 */
typedef struct pb_push_s pb_push_t;


/**
 * @typedef pb_phone_t
 * @brief Type definition of the structure pb_phone_s
 */
typedef struct pb_phone_s pb_phone_t;


/**
 * @typedef pb_device_t
 * @brief Type definition of the structure pb_device_s
 */
typedef struct pb_device_s pb_device_t;


/**
 * @typedef pb_devices_t
 * @brief Type definition of the structure pb_devices_s
 */
typedef struct pb_devices_s pb_devices_t;


/**
 * @typedef pb_browser_t
 * @brief Type definition of the structure pb_browser_s
 */
typedef struct pb_browser_s pb_browser_t;

/**
 * @ingroup  pb_config
 * @typedef  pb_config_t
 * @struct   pb_config_s
 * @brief    Opaque structure containing the value of the configuration
 */
typedef struct pb_config_s  pb_config_t;


/**
 * @typedef pb_user_t
 * @brief Type definition of the structure pb_user_s
 */
typedef struct pb_user_s pb_user_t;

/**
 * @brief HTTP codes definition
 */
typedef enum http_code_e {
    // Unknonwn error
    HTTP_UNKNOWN_CODE                    = 0,  ///< Unknown error

    // 1xx Informational
    HTTP_CONTINUE                        = 100, ///< Continue
    HTTP_SWITCHING_PROTOCOLS             = 101, ///< Switching protocols
    HTTP_PROCESSING                      = 102, ///< Processing
    
    
    // 2xx Success
    HTTP_OK                              = 200, ///< Ok
    HTTP_CREATED                         = 201, ///< Created
    HTTP_ACCEPTED                        = 202, ///< Accepted
    HTTP_NON_AUTHORITATIVE_INFORMATION   = 203, ///< Non authoritative information
    HTTP_NO_CONTENT                      = 204, ///< No content
    HTTP_RESET_CONTENT                   = 205, ///< Reset content
    HTTP_PARTIAL_CONTENT                 = 206, ///< Partial content
    HTTP_MULTI_STATUS                    = 207, ///< Multi status
    HTTP_ALREADY_REPORTED                = 208, ///< Already reported
    HTTP_IM_USED                         = 226, ///< I'm used
    
    
    // 3xx Redirection
    HTTP_MULTIPLE_CHOICES                = 300, ///< Multiple choices
    HTTP_MOVED_PERMANENTLY               = 301, ///< Moved permanently
    HTTP_FOUND                           = 302, ///< Found
    HTTP_SEE_OTHER                       = 303, ///< See other
    HTTP_NOT_MODIFIED                    = 304, ///< Not modified
    HTTP_USE_PROXY                       = 305, ///< Use proxy
    HTTP_SWITCH_PROXY                    = 306, ///< Switch proxy
    HTTP_TEMPORARY_REDIRECT              = 307, ///< Temporary redirect
    HTTP_PERMANENT_REDIRECT              = 308, ///< Permanent redirect
    
    
    // 4xx Error
    HTTP_BAD_REQUEST                     = 400, ///< Bad request
    HTTP_UNAUTHORIZED                    = 401, ///< Unauthorized
    HTTP_PAYMENT_REQUIRED                = 402, ///< Payment required
    HTTP_FORBIDDEN                       = 403, ///< Forbidden
    HTTP_NOT_FOUND                       = 404, ///< Not found
    HTTP_METHOD_NOT_ALLOWED              = 405, ///< Method not allowed
    HTTP_NOT_ACCEPTABLE                  = 406, ///< Not acceptable
    HTTP_PROXY_AUTHENTICATION_REQUIRED   = 407, ///< Proxy authentication required
    HTTP_REQUEST_TIMEOUT                 = 408, ///< Request timeout
    HTTP_CONFLICT                        = 409, ///< Conflict
    HTTP_GONE                            = 410, ///< Gone
    HTTP_LENGTH_REQUIRED                 = 411, ///< Length required
    HTTP_PRECONDITION_FAILED             = 412, ///< Precondition failed
    HTTP_PAYLOAD_TOO_LARGE               = 413, ///< Payload too large
    HTTP_URI_TOO_LONG                    = 414, ///< Uri too long
    HTTP_UNSUPPORTED_MEDIA_TYPE          = 415, ///< Unsupported media type
    HTTP_RANGE_NOT_SATISFIABLE           = 416, ///< Range not satisfiable
    HTTP_EXPECTATION_FAILED              = 417, ///< Expectation failed
    HTTP_I_M_A_TEAPOT                    = 418, ///< I'm a teapot
    HTTP_MISDIRECTED_REQUEST             = 421, ///< Misdirected request
    HTTP_UNPROCESSABLE_ENTITY            = 422, ///< Unprocessable entity
    HTTP_LOCKED                          = 423, ///< Locked
    HTTP_FAILED_DEPENDENCY               = 424, ///< Failed dependency
    HTTP_UPGRADE_REQUIRED                = 426, ///< Upgrade required
    HTTP_PRECONDITION_REQUIRED           = 428, ///< Precondition required
    HTTP_TOO_MANY_REQUESTS               = 429, ///< Too many requests
    HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE = 431, ///< Request header fields too large
    HTTP_UNAVAILABLE_FOR_LEGAL_REASONS   = 451, ///< Unavailable for legal reasons
    
    
    // 5xx Server Error
    HTTP_INTERNAL_SERVER_ERROR           = 500, ///< Internal server error
    HTTP_NOT_IMPLEMENTED                 = 501, ///< Not implemented
    HTTP_BAD_GATEWAY                     = 502, ///< Bad gateway
    HTTP_SERVICE_UNAVAILABLE             = 503, ///< Service unavailable
    HTTP_GATEWAY_TIMEOUT                 = 504, ///< Gateway timeout
    HTTP_HTTP_VERSION_NOT_SUPPORTED      = 505,  ///< HTTP version not supported
    HTTP_VARIANT_ALSO_NEGOTIATES         = 506, ///< Variant also negotiates
    HTTP_INSUFFICIENT_STORAGE            = 507, ///< Insufficient storage
    HTTP_LOOP_DETECTED                   = 508, ///< Loop detected
    HTTP_NOT_EXTENDED                    = 510, ///< Not extended
    HTTP_NETWORK_AUTHENTICATION_REQUIRED = 511 ///< Network authentication required
} http_code_t;


/**
 * @defgroup   pb_session Pushbullet session
 * @{
 */

/**
 * @brief      Initiate the different librairies
 *
 * @return     On success: zero
 * @return     On error: non-zero integer
 */
int pb_init(void);

/**
 * @brief      Terminate the different librairies
 */
void pb_term(void);

/**
 * @}
 */


/**
 * @defgroup   pb_devices Pushbullet devices
 * @{
 */

pb_device_t* pb_device_new();
int pb_device_ref(pb_device_t* p_device);
int pb_device_unref(pb_device_t* p_device);

pb_devices_t* pb_devices_new();
int pb_devices_ref(pb_devices_t* p_devices);
int pb_devices_unref(pb_devices_t* p_devices);

/**
 * @brief      Get the devices informations and stores it int a linked list in the user structure
 *
 * @param[in]  user  The user in which we store the devices
 *
 * @return     HTTP status code
 */
unsigned short pb_user_retrieve_devices(pb_user_t *user);


/**
 * @brief      Clear all the devices list of the given user
 *
 * @param      user  The user's
 */
void pb_devices_free_list(const pb_user_t *user);


/**
 * @brief      Get the identification of a device from its name
 *
 * @param[in]  user  The user that have the devices
 * @param[in]  name  The name of the device
 *
 * @return     The device's identification
 */
WARN_UNUSED_RESULT const char* pb_devices_get_iden_from_name(const pb_devices_t *devices, const char *nickname);

/**
 * @}
 */


/**
 * @defgroup   pb_pushes Pushbullet push functions
 * @{
 */

/**
 * @brief      Send a note
 *
 * @param[out] result           The buffer where we store the JSON response
 * @param[in]  note             The note's informations (title, body)
 * @param[in]  device_nickname  The device nickname
 * @param[in]  user             The user that sends the note
 *
 * @return     The HTTP status code to the \a pb_requests_post
 */
http_code_t pb_push_note(char *result, size_t *result_sz, const pb_note_t note, const char *device_nickname, const pb_user_t* user);

/**
 * @brief      Send a link
 *
 * @param[out] result           The buffer where we store the JSON response
 * @param[in]  link             The link's informations (title, body, URL)
 * @param[in]  device_nickname  The device nickname
 * @param[in]  user             The user that sends the link
 *
 * @return     The HTTP status code to the \a pb_requests_post
 */
http_code_t pb_push_link(char *result, size_t *result_sz, const pb_link_t link, const char *device_nickname, const pb_user_t* user);

/**
 * @brief      Send a file on the server
 *
 * @param      result           The result
 * @param      file             The file
 * @param[in]  device_nickname  The device nickname
 * @param[in]  user             The user
 *
 * @return      The HTTP status code to the \a pb_requests_post
 */
http_code_t pb_push_file(char *result, size_t *result_sz, pb_file_t *file, const char *device_nickname, const pb_user_t* user);

/**
 * @}
 */

/**
 * @defgroup  pb_user  Pushbullet user
 * @{
 */

/**
 * @brief      Create a new user
 * 
 * @return     Newly allocated user
 * 
 * @note       To free the user, use pb_user_unref
 */
WARN_UNUSED_RESULT pb_user_t* pb_user_new(void);

/**
 * @brief      Increase the user's reference counter
 *
 * @return     On success: zero
 * @return     On error: non-zero integer
 */
int pb_user_ref(pb_user_t* p_user);

/**
 * @brief      Decrease the user's reference counter
 * @details    When the reference counter hits zero, the ressource are freed.
 *
 * @param      p_user  The p user
 *
 * @return     On success: zero
 * @return     On error: non-zero integer
 */
int pb_user_unref(pb_user_t* p_user);

/**
 * @brief      Check if the user is active or not
 *
 * @param[in]  p_user  Pointer to the user
 *
 * @return     True if the user is active. Otherwise it returns false.
 */
unsigned char pb_user_is_active(const pb_user_t* p_user);

/**
 * @brief      Get the user's name
 *
 * @param[in]  p_user  Pointer to the user
 *
 * @return     The user's name or NULL
 */
char* pb_user_get_name(const pb_user_t* p_user);

/**
 * @brief      Get the user's email
 *
 * @param[in]  p_user  Pointer to the user
 *
 * @return     The user's email or NULL
 */
char* pb_user_get_email(const pb_user_t* p_user);

/**
 * @brief      Get the user's identification
 *
 * @param[in]  p_user  Pointer to the user
 *
 * @return     The identification string or NULL
 */
char* pb_user_get_iden(const pb_user_t* p_user);


/**
 * @brief      Set the configuration for a user
 *
 * @param      p_user    The p user
 * @param      p_config  The p configuration
 *
 * @return     On success: zero
 * @return     On error: non-zero integer
 */
int pb_user_set_config(pb_user_t* p_user, pb_config_t* p_config);

/**
 * @brief      Get the configuration for a user
 *
 * @param      p_user    Pointer to the user
 *
 * @return     On success: pointer to the configuration
 * @return     On error: NULL
 */
WARN_UNUSED_RESULT pb_config_t* pb_user_get_config(const pb_user_t* p_user);

/**
 * @brief      Get the devices associated to a user
 *
 * @param      p_user    Pointer to the user
 *
 * @return     On success: pointer to the devices structure (can contain multiple devices)
 * @return     On error: NULL
 */
WARN_UNUSED_RESULT pb_devices_t* pb_user_get_devices(const pb_user_t* p_user);

/**
 * @brief      Get the number of devices associated to a user
 *
 * @param      p_user    Pointer to the user
 *
 * @return     The number of devices active
 */
WARN_UNUSED_RESULT size_t pb_user_get_number_active_devices(const pb_user_t *p_user);


const char* pb_user_get_device_iden_from_name(const pb_user_t *p_user, const char* nickname);

/**
 * @brief      Get the user's informations from the Pushbullet servers.
 *
 * @param      p_user    Pointer to the user
 *
 * @return     The HTTP code of the Curl request
 */
http_code_t pb_user_get_info(pb_user_t *p_user);

/**
 * @}
 */

/**
 * @defgroup  pb_config  Pushbullet configuration
 * @{
 */

/**
 * @brief      Create a new configuration
 * 
 * @return     On success, a newly allocated configuration.
 * @return     On error, NULL.
 * 
 * @note       To free the configuration, use pb_config_unref
 */
WARN_UNUSED_RESULT pb_config_t* pb_config_new(void);

/**
 * @brief      Increase the reference counter of the configuration structure
 *
 * @param[in]  p_config  Pointer to the configuration
 */
int pb_config_ref(pb_config_t* p_config);

/**
 * @brief      Decrease the reference counter of the configuration structure
 *
 * @param[in]  p_config  Pointer to the configuration
 */
int pb_config_unref(pb_config_t* p_config);

/**
 * @brief      Set the configuration's https_proxy value
 *
 * @param      p_config    Pointer to the configuration
 * @param[in]  https_proxy  The https proxy
 *
 * @return     On success: zero
 * @return     On error: non-zero integer
 */
int pb_config_set_https_proxy(pb_config_t* p_config, const char* https_proxy);

/**
 * @brief      Set the configuration's timeout value
 *
 * @param      p_config    Pointer to the configuration
 * @param[in]  timeout     Request's timeout
 *
 * @return     On success: zero
 * @return     On error: non-zero integer
 */
int pb_config_set_timeout(pb_config_t* p_config, const long timeout);

/**
 * @brief      Set the configuration's token_key value
 *
 * @param      p_config    Pointer to the configuration
 * @param[in]  token_key   The Pushbullet token key
 *
 * @return     On success: zero
 * @return     On error: non-zero integer
 */
int pb_config_set_token_key(pb_config_t* p_config, const char* token_key);

/**
 * @brief      Retrieve the https_proxy from the configuration
 *
 * @param[in]  p_config  Pointer to the configuration
 *
 * @return     On success: pointer to the https_proxy value
 * @return     On error: NULL
 */
WARN_UNUSED_RESULT char* pb_config_get_https_proxy(const pb_config_t* p_config);

/**
 * @brief      Retrieve the timeout from the configuration
 *
 * @param[in]  p_config  Pointer to the configuration
 *
 * @return     On success: the timeout value
 * @return     On error: -1
 */
WARN_UNUSED_RESULT long pb_config_get_timeout(const pb_config_t* p_config);

/**
 * @brief      Retrieve the token_key from the configuration
 *
 * @param[in]  p_config  Pointer to the configuration
 *
 * @return     On success: pointer to the token_key value
 * @return     On error: NULL
 */
WARN_UNUSED_RESULT char* pb_config_get_token_key(const pb_config_t* p_config);

/**
 * @brief      Fill the configuration structure using the given JSON file path
 *
 * @param      p_config       Pointer to the configuration
 * @param[in]  json_filepath  The json filepath
 *
 * @return     On success: 0
 * @return     On error: non-zero integer
 */
int pb_config_from_json_file(pb_config_t* p_config, const char *json_filepath);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif          // __PUSHBULLET_H__