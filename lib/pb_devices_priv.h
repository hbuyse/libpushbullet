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

typedef struct pb_device_s pb_device_t;

typedef struct pb_devices_s {
    ssize_t nb_active;                  ///< Active devices' list  size
    pb_device_t* list;       ///< Devices' list
    int ref;                    ///< Reference counter
} pb_devices_t;


#ifdef __cplusplus
}
#endif

#endif // __PB_DEVICES_PRIV__
