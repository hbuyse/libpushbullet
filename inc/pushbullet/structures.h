/**
 * @file structures.h
 * @author hbuyse
 * @date 11/05/2016
 *
 * @brief  Strutures needed by the program
 */


#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__


#include <json/json.h>          // json_object

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Maximum length of a MIME type
 * @details According to RFC 4288 "Media Type Specifications and Registration Procedures", type (eg. "application") and
 * subtype (eg "vnd...") both can
 *          be max 127 characters. (256 - 0x100)
 */
#define     MIME_TYPE_MAX_LENGTH 0x100


/**
 * @enum pb_device_icon
 * @brief Device's icon
 */
enum pb_device_icon {
    ICON_DESKTOP,          ///< Desktop
    ICON_BROWSER,          ///< Browser
    ICON_WEBSITE,          ///< Website
    ICON_LAPTOP,          ///< Laptop
    ICON_TABLET,          ///< Tablet
    ICON_PHONE,          ///< Phone
    ICON_WATCH,          ///< Watch
    ICON_SYSTEM,          ///< System
    ICON_DEVICE          ///< Device
};


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
 * @typedef pb_upload_request_t
 * @brief Type definition of the structure pb_upload_request_s
 */
typedef struct pb_upload_request_s pb_upload_request_t;


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
 * @typedef pb_browser_t
 * @brief Type definition of the structure pb_browser_s
 */
typedef struct pb_browser_s pb_browser_t;


/**
 * @typedef pb_user_t
 * @brief Type definition of the structure pb_user_s
 */
typedef struct pb_user_s pb_user_t;


/**
 * @struct pb_note_s
 * @brief Structure containing all the informations concerning a PushBullet note
 */
struct pb_note_s {
    char *title;          ///< Push's title
    char *body;          ///< Push's body
};


/**
 * @struct pb_link_s
 * @brief Structure containing all the informations concerning a PushBullet link
 */
struct pb_link_s {
    char *title;          ///< Push's title
    char *body;          ///< Push's body
    char *url;          ///< Push's url
};


/**
 * @struct pb_upload_request_s
 * @brief Structure containing all the informations concerning a PushBullet Upload request
 */
struct pb_upload_request_s {
    char *file_path;          ///< File path
    char *file_name;          ///< File name
    char file_type[MIME_TYPE_MAX_LENGTH];          ///< File's MIME type
};


/**
 * @struct pb_push_s
 * @brief Structure containing all the informations concerning a PushBullet push
 */
struct pb_push_s {
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
};


/**
 * @struct pb_phone_s
 * @brief Structure containing all the informations concerning a PushBullet phone
 */
struct pb_phone_s {
    unsigned char active;           ///< Activity of the phone
    const char *iden;           ///< Phone's indentification
    double created;           ///< Phone's creation
    double modified;           ///< Phone's modification
    const char *nickname;           ///< Phone's nickname
    unsigned char generated_nickname;          ///< Has the nickname been generated automatically?
    const char *manufacturer;          ///< Phone's manufacturer
    const char *model;          ///< Phone's model
    short app_version;          ///< Phone's application version
    json_object *fingerprint;          ///< Phone's fingerprint
    const char *push_token;          ///< Phone's push token
    unsigned char has_sms;          ///< Can we send SMS?
    unsigned char has_mms;          ///< Can we send MMS?
    const char *icon;          ///< Phone's icon
    const char *remote_files;          ///< Remote files
};


/**
 * @struct pb_browser_s
 * @brief Structure containing all the informations concerning a PushBullet browser
 */
struct pb_browser_s {
    unsigned char active;          ///< Activity of the browser
    const char *iden;          ///< Browser's indentification
    double created;          ///< Browser's creation
    double modified;          ///< Browser's modifications
    const char *nickname;          ///< Browser's nickname
    const char *manufacturer;          ///< Browser's manufacturer
    const char *model;          ///< Browser's model
    short app_version;          ///< Browser's application version
    const char *icon;          ///< Browser's icon
};


/**
 * @struct pb_device_s
 * @brief Element of a linked list containing either a browser or a phone
 */
struct pb_device_s {
    unsigned char type_device;          ///< The type of the device
    union {
        pb_phone_t phone;              ///< Phone device if the type is a phone
        pb_browser_t browser;          ///< Browser device if the type is a browser
    };

    pb_device_t *next;                   ///< Pointer to the next
};


/**
 * @struct pb_user_s
 * @brief Contains the user informations.
 */
struct pb_user_s {
    const char *token_key;          ///< The user's token_key
    unsigned char active;          ///< Boolean that tells if the user is active or not
    double created;             ///< Epoch time when the user was created
    double modified;            ///< Epoch time when the user was last modified
    const char *email;          ///< The user's email
    const char *email_normalized;          ///< The user's email normalized
    const char *iden;           ///< The user's identification
    const char *image_url;          ///< The URL to the user's photo
    const char *name;           ///< The user's name
    int max_upload_size;          ///< The maximum size of a file the user can upload in bytes
    json_object *config;            ///< Configuration from the config file
    pb_device_t *devices;          ///< The list of active devices
};

#ifdef __cplusplus
}
#endif

#endif          // __PB_STRUCTURES_H__