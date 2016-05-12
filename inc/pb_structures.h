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

enum {
    TYPE_ANDROID = 0,
    TYPE_IPHONE,
    TYPE_CHROME,
    TYPE_FIREFOX,
    TYPE_DEVICE
};


/**
 * \typedef PB_phone_t
 */
typedef struct PB_phone_s PB_phone_t;


/**
 * \typedef PB_device_t
 */
typedef struct PB_device_s PB_device_t;


/**
 * \typedef PB_browser_t
 */
typedef struct PB_browser_s PB_browser_t;


/**
 * \typedef PB_user_t
 */
typedef struct PB_user_s PB_user_t;


/**
 * \struct PB_phone_s
 * \brief Structure containing all the informations concerning a PushBullet phone
 */
struct PB_phone_s {
    unsigned char active;          // true,
    const char *iden;          // "ujEIL5AaxhYsjAyl73vD3s",
    double created;          // 1459865409.299245,
    double modified;          // 1459865409.2992482,
    const char *type;          // "android",
    const char *kind;          // "android",
    const char *nickname;          // "LGE Nexus 5X",
    unsigned char generated_nickname;          // true,
    const char *manufacturer;          // "LGE",
    const char *model;          // "Nexus 5X",
    short app_version;          // 256,
    json_object *fingerprint;          // "{\"android_id\":\"f48298904b847bd0\"}",
    const char *push_token;          // "fcH5ndd7TuY:APA91bHaNqTASWFRP7dOg5YUfEhuTNv1AB-zBljo_mHzB-zUtUxAPtYkEUwKz69V0dG5vWJFhUb52x7sbEwCZbAYjxYpmKMbacfEtMi1Dvm-eqdKWzJEJQzYWTD5lyADztPcFHIRYYEQ",
    unsigned char pushable;          // true,
    unsigned char has_sms;          // true,
    unsigned char has_mms;          // true,
    const char *icon;          // "phone",
    const char *remote_files;          // "disabled"
};


/**
 * \struct PB_browser_s
 * \brief Structure containing all the informations concerning a PushBullet browser
 */
struct PB_browser_s {
    unsigned char active;          // true,
    const char *iden;          // "ujEIL5AaxhYsjAaqJB5LXg",
    double created;          // 1427959507.662864,
    double modified;          // 1427959507.662869,
    const char *type;          // "chrome",
    const char *kind;          // "chrome",
    const char *nickname;          // "Chrome",
    const char *manufacturer;          // "Google",
    const char *model;          // "Chrome",
    short app_version;          // 188,
    unsigned char pushable;          // true,
    const char *icon;          // "browser"
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
    int max_upload_size;
    PB_device_t *devices;          ///< The list of active devices
};

#ifdef __cplusplus
}
#endif

#endif          // __PB_STRUCTURES_H__