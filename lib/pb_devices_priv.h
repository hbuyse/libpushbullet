#ifndef __PB_DEVICES_PRIV__
#define __PB_DEVICES_PRIV__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @def MAX_SIZE_BUF
 * Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF    0x1000


/**
 * @def DEVICES_JSON_KEY
 * String defining the "devices" key
 */
#define DEVICES_JSON_KEY    "devices"


/**
 * @def ACTIVE_JSON_KEY
 * String defining the "active" key
 */
#define ACTIVE_JSON_KEY     "active"


/**
 * @def JSON_KEY_ICON
 * String defining the "icon" key
 */
#define JSON_KEY_ICON       "icon"


/**
 * @def DESKTOP_ICON
 * String defining the "desktop" key
 */
#define DESKTOP_ICON        "desktop"


/**
 * @def BROWSER_ICON
 * String defining the "browser" key
 */
#define BROWSER_ICON        "browser"


/**
 * @def WEBSITE_ICON
 * String defining the "website" key
 */
#define WEBSITE_ICON        "website"


/**
 * @def LAPTOP_ICON
 * String defining the "laptop" key
 */
#define LAPTOP_ICON         "laptop"


/**
 * @def TABLET_ICON
 * String defining the "tablet" key
 */
#define TABLET_ICON         "tablet"


/**
 * @def PHONE_ICON
 * String defining the "phone" key
 */
#define PHONE_ICON          "phone"


/**
 * @def WATCH_ICON
 * String defining the "watch" key
 */
#define WATCH_ICON          "watch"


/**
 * @def SYSTEM_ICON
 * String defining the "system" key
 */
#define SYSTEM_ICON         "system"


/**
 * @brief      Macro to associate a key in a structure
 *
 * @param      type  The type
 * @param      var   The pointer we fill
 * @param      k     The JSON key
 */
#define     JSON_ASSOCIATE_BOOL(var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) var->k = json_node_get_boolean(member_node); } while(0)

#define     JSON_ASSOCIATE_STR(var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) var->k = json_node_dup_string(member_node); } while(0)

#define     JSON_ASSOCIATE_INT(var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) var->k = json_node_get_int(member_node); } while(0)

#define     JSON_ASSOCIATE_DOUBLE(var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) var->k = json_node_get_double(member_node); } while(0)


/**
 * @enum pb_device_icon
 * @brief Device's icon
 */
typedef enum {
    ICON_DESKTOP,          ///< Desktop
    ICON_BROWSER,          ///< Browser
    ICON_WEBSITE,          ///< Website
    ICON_LAPTOP,          ///< Laptop
    ICON_TABLET,          ///< Tablet
    ICON_PHONE,          ///< Phone
    ICON_WATCH,          ///< Watch
    ICON_SYSTEM,          ///< System
    ICON_DEVICE          ///< Device
} pb_device_icon;


/**
 * @struct pb_phone_s
 * @brief Structure containing all the informations concerning a PushBullet phone
 */
typedef struct pb_phone_s {
    unsigned char active;           ///< Activity of the phone
    char *iden;           ///< Phone's indentification
    double created;           ///< Phone's creation
    double modified;           ///< Phone's modification
    char *nickname;           ///< Phone's nickname
    unsigned char generated_nickname;          ///< Has the nickname been generated automatically?
    char *manufacturer;          ///< Phone's manufacturer
    char *model;          ///< Phone's model
    short app_version;          ///< Phone's application version
    char *fingerprint;          ///< Phone's fingerprint
    char *push_token;          ///< Phone's push token
    unsigned char has_sms;          ///< Can we send SMS?
    unsigned char has_mms;          ///< Can we send MMS?
    char *icon;          ///< Phone's icon
    char *remote_files;          ///< Remote files
} pb_phone_t;


/**
 * @struct pb_browser_s
 * @brief Structure containing all the informations concerning a PushBullet browser
 */
typedef struct pb_browser_s {
    unsigned char active;          ///< Activity of the browser
    char *iden;          ///< Browser's indentification
    double created;          ///< Browser's creation
    double modified;          ///< Browser's modifications
    char *nickname;          ///< Browser's nickname
    char *manufacturer;          ///< Browser's manufacturer
    char *model;          ///< Browser's model
    short app_version;          ///< Browser's application version
    char *icon;          ///< Browser's icon
} pb_browser_t;


/**
 * @struct pb_device_s
 * @brief Element of a linked list containing either a browser or a phone
 */
typedef struct pb_device_s {
    pb_device_icon type;            ///< The type of the device
    union {
        pb_phone_t phone;           ///< Phone device if the type is a phone
        pb_browser_t browser;       ///< Browser device if the type is a browser
    };

    int ref;                        ///< Reference counter
    struct pb_device_s *next;              ///< Pointer to the next
} pb_device_t;


typedef struct pb_devices_s {
    ssize_t nb;                  ///< Devices' list size
    pb_device_t* list;       ///< Devices' list
    int ref;                    ///< Reference counter
} pb_devices_t;


#ifdef __cplusplus
}
#endif

#endif // __PB_DEVICES_PRIV__