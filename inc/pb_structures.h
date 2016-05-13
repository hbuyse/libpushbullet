/**
 * @file pb_structures.h
 * @author hbuyse
 * @date 11/05/2016
 *
 * @brief  Strutures needed by the program
 */


#ifndef __PB_STRUCTURES_H__
#define __PB_STRUCTURES_H__


#include <json/json.h>          // json_object


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum PB_device_type
 * \brief Device's type
 */
enum PB_device_type {
    TYPE_ANDROID = 0,   ///< Android
    TYPE_IPHONE,   ///< Iphone
    TYPE_CHROME,   ///< Chrome
    TYPE_FIREFOX,   ///< Firefox
    TYPE_DEVICE   ///< Device
};


/**
 * \typedef PB_push_t
 * \brief Type definition of the structure PB_push_s
 */
typedef struct PB_push_s PB_push_t;


/**
 * \typedef PB_phone_t
 * \brief Type definition of the structure PB_phone_s
 */
typedef struct PB_phone_s PB_phone_t;


/**
 * \typedef PB_device_t
 * \brief Type definition of the structure PB_device_s
 */
typedef struct PB_device_s PB_device_t;


/**
 * \typedef PB_browser_t
 * \brief Type definition of the structure PB_browser_s
 */
typedef struct PB_browser_s PB_browser_t;


/**
 * \typedef PB_user_t
 * \brief Type definition of the structure PB_user_s
 */
typedef struct PB_user_s PB_user_t;


/**
 * \struct PB_push_s
 * \brief Structure containing all the informations concerning a PushBullet push
 */
struct PB_push_s {
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
 * \struct PB_phone_s
 * \brief Structure containing all the informations concerning a PushBullet phone
 */
struct PB_phone_s {
    unsigned char active;           ///< Activity of the phone
    const char *iden;           ///< Phone's indentification
    double created;           ///< Phone's creation
    double modified;           ///< Phone's modification
    const char *type;           ///< Phone's type
    const char *kind;           ///< Phone's kind
    const char *nickname;           ///< Phone's nickname
    unsigned char generated_nickname;          ///< Has the nickname been generated automatically?
    const char *manufacturer;          ///< Phone's manufacturer
    const char *model;          ///< Phone's model
    short app_version;          ///< Phone's application version
    json_object *fingerprint;          ///< Phone's fingerprint
    const char *push_token;          ///< Phone's push token
    unsigned char pushable;     ///< Is the phone pushable?
    unsigned char has_sms;          ///< Can we send SMS?
    unsigned char has_mms;          ///< Can we send MMS?
    const char *icon;       ///< Phone's icon
    const char *remote_files;          ///< Remote files
};


/**
 * \struct PB_browser_s
 * \brief Structure containing all the informations concerning a PushBullet browser
 */
struct PB_browser_s {
    unsigned char active;       ///< Activity of the browser
    const char *iden;       ///< Browser's indentification
    double created;     ///< Browser's creation
    double modified;        ///< Browser's modification
    const char *type;       ///< Browser's type
    const char *kind;       ///< Browser's kind
    const char *nickname;       ///< Browser's nickname
    const char *manufacturer;       ///< Browser's manufacturer
    const char *model;      ///< Browser's model
    short app_version;      ///< Browser's application version
    unsigned char pushable;     ///< Is the browser pushable?
    const char *icon;       ///< Browser's icon
};


/**
 * \struct PB_device_s
 * \brief Element of a linked list containing either a browser or a phone
 */
struct PB_device_s {
    unsigned char type_device;          ///< The type of the device
    union {
        PB_phone_t phone;              ///< Phone device if the type is a phone
        PB_browser_t browser;          ///< Browser device if the type is a browser
    };

    PB_device_t *next;                   ///< Pointer to the next
};


/**
 * \struct PB_user_s
 * \brief Contains the user informations.
 */
struct PB_user_s {
    char *token_key;          ///< The user's token_key
    unsigned char active;          ///< Boolean that tells if the user is active or not
    double created;             ///< Epoch time when the user was created
    double modified;            ///< Epoch time when the user was last modified
    const char *email;          ///< The user's email
    const char *email_normalized;          ///< The user's email normalized
    const char *iden;           ///< The user's identification
    const char *image_url;          ///< The URL to the user's photo
    const char *name;           ///< The user's name
    int max_upload_size;        ///< The maximum size of a file the user can upload in bytes
    PB_device_t *devices;          ///< The list of active devices
};

#ifdef __cplusplus
}
#endif

#endif          // __PB_STRUCTURES_H__