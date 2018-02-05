/**
 * @file note.c
 * @author hbuyse
 * @date 12/05/2016
 */

#ifndef __PB_PUSHES_PRIV_H__
#define __PB_PUSHES_PRIV_H__

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
 * \brief    JSON key to get the future url of the file
 * @details  Field containing the URL where the file will be available after it is uploaded.
 */
#define     JSON_KEY_FILE_URL           "file_url"


/**
 * \brief    JSON key to get the upload url of the file
 * @details  Field containing the URL to POST the file to.
 *           The file must be posted using multipart/form-data encoding.
 */
#define     JSON_KEY_UPLOAD_URL         "upload_url"


/**
 * @struct pb_note_s
 * @brief Structure containing all the informations concerning a PushBullet note
 */
typedef struct pb_note_s {
    char *title;          ///< Push's title
    char *body;          ///< Push's body
} pb_note_t;


/**
 * @struct pb_link_s
 * @brief Structure containing all the informations concerning a PushBullet link
 */
typedef struct pb_link_s {
    char *title;          ///< Push's title
    char *body;          ///< Push's body
    char *url;          ///< Push's url
} pb_link_t;


/**
 * @struct pb_file_s
 * @brief Structure containing all the informations concerning a PushBullet Upload request for a file
 */
typedef struct pb_file_s {
    char *title;          ///< Push's title
    char *body;          ///< Push's body
    char *file_path;          ///< File path
    char *file_name;          ///< File name
    char *file_type;          ///< File's MIME type
    char *file_url;          ///< File url
    char *upload_url;          ///< File upload url
} pb_file_t;


/**
 * @struct pb_push_s
 * @brief Structure containing all the informations concerning a PushBullet push
 */
typedef struct pb_push_s {
    unsigned char active;          ///< Push's activity
    const char *body;          ///< Push's body
    double created;          ///< Push's creation
    const char *direction;          ///< Push's direction
    unsigned char dismissed;          ///< Is the push dismissed?
    const char *iden;          ///< Push's identification
    double modified;          ///< Push's last modification
    const char *receiver_email;          ///< Receiver email of the push
    const char *receiver_email_normalized;          ///< Receiver email normalized of the push
    const char *receiver_iden;          ///< Receiver identification of the push
    const char *sender_email;          ///< Sender's email of the push
    const char *sender_email_normalized;          ///< Sender's email normalized of the push
    const char *sender_iden;          ///< Sender's identification of the push
    const char *sender_name;          ///< Sender's name of the push
    const char *title;          ///< Push's title
    const char *type;          ///< Push's type
} pb_push_t;



#ifdef __cplusplus
}
#endif

#endif          // __PB_PUSHES_PRIV_H__