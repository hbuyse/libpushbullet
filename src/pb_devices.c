/**
 * @file devices.c
 * @author hbuyse
 * @date 08/05/2016
 */
#include <stdlib.h>          // free
#include <string.h>          // strcmpt
#include <json-glib/json-glib.h>          // JsonObject, json_tokener_parse, json_object_object_foreach, json_object_get_array,
                                // array_list

#include "pb_utils.h"             // iprintf, eprintf, cprintf, gprintf
#include "pb_internal.h"         // pb_requests_get
#include "pushbullet.h"


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
#define     JSON_ASSOCIATE(type, var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) {var->k = json_node_get_ ## type(member_node); } } while(0)



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
 * @struct pb_phone_s
 * @brief Structure containing all the informations concerning a PushBullet phone
 */
typedef struct pb_phone_s {
    unsigned char active;           ///< Activity of the phone
    const char *iden;           ///< Phone's indentification
    double created;           ///< Phone's creation
    double modified;           ///< Phone's modification
    const char *nickname;           ///< Phone's nickname
    unsigned char generated_nickname;          ///< Has the nickname been generated automatically?
    const char *manufacturer;          ///< Phone's manufacturer
    const char *model;          ///< Phone's model
    short app_version;          ///< Phone's application version
    const char *fingerprint;          ///< Phone's fingerprint
    const char *push_token;          ///< Phone's push token
    unsigned char has_sms;          ///< Can we send SMS?
    unsigned char has_mms;          ///< Can we send MMS?
    const char *icon;          ///< Phone's icon
    const char *remote_files;          ///< Remote files
} pb_phone_t;


/**
 * @struct pb_browser_s
 * @brief Structure containing all the informations concerning a PushBullet browser
 */
typedef struct pb_browser_s {
    unsigned char active;          ///< Activity of the browser
    const char *iden;          ///< Browser's indentification
    double created;          ///< Browser's creation
    double modified;          ///< Browser's modifications
    const char *nickname;          ///< Browser's nickname
    const char *manufacturer;          ///< Browser's manufacturer
    const char *model;          ///< Browser's model
    short app_version;          ///< Browser's application version
    const char *icon;          ///< Browser's icon
} pb_browser_t;


/**
 * @struct pb_device_s
 * @brief Element of a linked list containing either a browser or a phone
 */
typedef struct pb_device_s {
    unsigned char type_device;          ///< The type of the device
    union {
        pb_phone_t phone;              ///< Phone device if the type is a phone
        pb_browser_t browser;          ///< Browser device if the type is a browser
    };

    pb_device_t *next;                   ///< Pointer to the next
} pb_device_t;


/**
 * @brief      Fill the browser structure with the informations contained in the JSON object
 *
 * @param[out]  browser  The browser structure we fill
 * @param[in]  json_obj  The JSON object
 */
static void _get_browser_device(JsonObject *object __attribute__((unused)),
                                const gchar *member_name,
                                JsonNode *member_node,
                                gpointer userdata
                                );



/**
 * @brief      Fill the phone structure with the informations contained in the JSON object
 *
 * @param[out]  phone  The phone structure we fill
 * @param[in]  json_obj  The JSON object
 */
static void _get_phone_device(JsonObject *object __attribute__((unused)),
                              const gchar *member_name,
                              JsonNode *member_node,
                              gpointer userdata
                              );


#ifdef __DEBUG__
/**
 * @brief      Display on stdout the informations about the phone
 *
 * @param[in]  phone  The phone
 */
static void _dump_phone_infos(const pb_phone_t phone);


/**
 * @brief      Display on stdout the informations about the browser
 *
 * @param[in]  browser  The browser
 */
static void _dump_browser_infos(const pb_browser_t browser);


/**
 * @brief      Display all the devices of a given user
 *
 * @param      user  The user
 */
static void _dump_devices_list(const pb_user_t *user);
#endif

static void _fill_devices_list(JsonArray *arr __attribute__((unused)),
                               guint idx,
                               JsonNode *node_arr,
                               gpointer userdata);

unsigned short pb_devices_get_list(pb_user_t *user)
{
    // CURL results
    char                *result             = NULL;
    size_t              result_sz           = 0;
    unsigned short      res                 = 0;


    // JSON variables
    JsonParser * parser                  = NULL;
    JsonNode* root                       = NULL;
    JsonNode* devices_node               = NULL;
    JsonArray* devices_arr               = NULL;
    JsonObject         *obj         = NULL;


    res = pb_requests_get(&result, &result_sz, API_URL_DEVICES, (pb_config_t*) pb_user_get_config(user));

    // If we do not have a 200 OK, we stop the function and we return the HTTP Status code
    if ( res == HTTP_OK )
    {
        root = json_parser_get_root(parser);
        obj = json_node_get_object(root);

        if ( json_object_has_member(obj, DEVICES_JSON_KEY) )
        {
            devices_node = json_object_get_member(obj, DEVICES_JSON_KEY);
            devices_arr = json_node_get_array(devices_node);

            json_array_foreach_element(devices_arr, _fill_devices_list, pb_user_get_devices_list(user));

            #ifdef __DEBUG__
            _dump_devices_list(user);
            #endif
        }
    }

    pb_free(result);

    return (res);
}



void pb_devices_free_list(const pb_user_t *user)
{
    pb_device_t     *devices = NULL;


    // Pass through the list
    for ( devices = pb_user_get_devices_list(user); devices != NULL; )
    {
        // Get the pointer
        pb_device_t* current_device = devices;


        // Pass to the next pointer
        devices   = devices->next;


        // Free the temporary pointer
        free(current_device);
    }

#ifdef __DEBUG__
    _dump_devices_list(user);
#endif
}



unsigned char pb_devices_get_number_active(const pb_user_t *user)
{
    unsigned char       i       = 0;
    pb_device_t         *devices    = NULL;

    for ( devices = pb_user_get_devices_list(user), i = 0; devices != NULL; devices = devices->next, ++i)
    {
        ;
    }

    return (i);
}



const char* pb_devices_get_iden_from_name(const pb_user_t   *user,
                                  const char        *name
                                  )
{
    pb_device_t     *devices = NULL;


    if ( ! name )
    {
        return (NULL);
    }

    for ( devices = pb_user_get_devices_list(user); devices != NULL; devices = devices->next )
    {
        switch ( devices->type_device )
        {
            case ICON_PHONE:

                if ( strcmp(devices->phone.nickname, name) == 0 )
                {
                    return (devices->phone.iden);
                }

                break;

            case ICON_BROWSER:

                if ( strcmp(devices->browser.nickname, name) == 0 )
                {
                    return (devices->browser.iden);
                }

                break;

            default:
                eprintf("Unknown type...\n");
        }
    }

    return (NULL);
}

static void _get_browser_device(JsonObject *object __attribute__((unused)),
                                const gchar *member_name,
                                JsonNode *member_node,
                                gpointer userdata
                                )
{
    pb_browser_t        *browser = (pb_browser_t*) userdata;

    if (JSON_NODE_HOLDS_VALUE(member_node))
    {
        JSON_ASSOCIATE(boolean, browser, active);
        JSON_ASSOCIATE(string, browser, iden);
        JSON_ASSOCIATE(double, browser, created);
        JSON_ASSOCIATE(double, browser, modified);
        JSON_ASSOCIATE(string, browser, nickname);
        JSON_ASSOCIATE(string, browser, manufacturer);
        JSON_ASSOCIATE(string, browser, model);
        JSON_ASSOCIATE(int, browser, app_version);
        JSON_ASSOCIATE(string, browser, icon);
    }
}

static void _get_phone_device(JsonObject *object __attribute__((unused)),
                              const gchar *member_name,
                              JsonNode *member_node,
                              gpointer userdata
                              )
{
    pb_phone_t        *phone = (pb_phone_t*) userdata;

    if (JSON_NODE_HOLDS_VALUE(member_node))
    {
        JSON_ASSOCIATE(boolean, phone, active);
        JSON_ASSOCIATE(string, phone, iden);
        JSON_ASSOCIATE(double, phone, created);
        JSON_ASSOCIATE(double, phone, modified);
        JSON_ASSOCIATE(string, phone, nickname);
        JSON_ASSOCIATE(boolean, phone, generated_nickname);
        JSON_ASSOCIATE(string, phone, manufacturer);
        JSON_ASSOCIATE(string, phone, model);
        JSON_ASSOCIATE(int, phone, app_version);
        JSON_ASSOCIATE(string, phone, push_token);
        JSON_ASSOCIATE(boolean, phone, has_sms);
        JSON_ASSOCIATE(boolean, phone, has_mms);
        JSON_ASSOCIATE(string, phone, icon);
        JSON_ASSOCIATE(string, phone, remote_files);
        JSON_ASSOCIATE(string, phone, fingerprint);       // JsonObject
    }
}


#ifdef __DEBUG__
static void _dump_phone_infos(const pb_phone_t phone)
{
    iprintf("%c%s - %s\n", phone.icon[0] - 32, phone.icon + 1, phone.iden);
    iprintf("\tactive : %u\n", phone.active);
    iprintf("\tcreated : %f\n", phone.created);
    iprintf("\tmodified : %f\n", phone.modified);
    iprintf("\tnickname : %s\n", phone.nickname);
    iprintf("\tgenerated_nickname : %u\n", phone.generated_nickname);
    iprintf("\tmanufacturer : %s\n", phone.manufacturer);
    iprintf("\tmodel : %s\n", phone.model);
    iprintf("\tapp_version : %hd\n", phone.app_version);
    iprintf("\tfingerprint : %s\n", phone.fingerprint);
    iprintf("\tpush_token : %s\n", phone.push_token);
    iprintf("\thas_sms : %u\n", phone.has_sms);
    iprintf("\thas_mms : %u\n", phone.has_mms);
    iprintf("\tremote_files : %s\n", phone.remote_files);
}

static void _dump_browser_infos(const pb_browser_t browser)
{
    iprintf("%c%s - %s\n", browser.icon[0] - 32, browser.icon + 1, browser.iden);
    iprintf("\tactive : %u\n", browser.active);
    iprintf("\tcreated : %f\n", browser.created);
    iprintf("\tmodified : %f\n", browser.modified);
    iprintf("\tnickname : %s\n", browser.nickname);
    iprintf("\tmanufacturer : %s\n", browser.manufacturer);
    iprintf("\tmodel : %s\n", browser.model);
    iprintf("\tapp_version : %hd\n", browser.app_version);
}

static void _dump_devices_list(const pb_user_t *user)
{
    pb_device_t     *devices = NULL;


    for ( devices = pb_user_get_devices_list(user); devices != NULL; devices = devices->next )
    {
        switch ( devices->type_device )
        {
            case ICON_PHONE:
                _dump_phone_infos(devices->phone);
                break;

            case ICON_BROWSER:
                _dump_browser_infos(devices->browser);
                break;

            default:
                eprintf("Unknown type...\n");
        }
    }
}
#endif


static void _fill_devices_list(JsonArray *arr __attribute__((unused)),
                               guint idx,
                               JsonNode *node_arr,
                               gpointer userdata)
{
    pb_device_t *devices_list = (pb_device_t*) userdata;
    JsonObject* node_obj = NULL;
    const char* device_type = NULL;

    #ifdef __DEBUG__
    iprintf("Device n°%d\n", idx);
    #endif

    if ( ! JSON_NODE_HOLDS_OBJECT(node_arr) )
    {
        eprintf("devices[%d] : The node does not contain an JsonObject\n", idx);
    }
    else if ( (node_obj = json_node_get_object(node_arr) ) == NULL)
    {
        eprintf("devices[%d] : Impossible to get the object from the node\n", idx);
    }
    else if ( ! json_object_has_member(node_obj, ACTIVE_JSON_KEY) )
    {
        eprintf("devices[%d] : The obj does not have the member \"%s\"\n", idx, ACTIVE_JSON_KEY);
    }
    else if ( ! json_object_get_boolean_member(node_obj, ACTIVE_JSON_KEY) )
    {
        eprintf("devices[%d] : Impossible to get the boolean member \"%s\" from object\n", idx, ACTIVE_JSON_KEY);
    }
    else if ( ! json_object_has_member(node_obj, JSON_KEY_ICON) )
    {
        eprintf("devices[%d] : The obj does not have the member \"%s\"\n", idx, JSON_KEY_ICON);
    }
    else if ( (device_type = json_object_get_string_member(node_obj, JSON_KEY_ICON)) == NULL )
    {
        eprintf("devices[%d] : Impossible to get the string member \"%s\" from object\n", idx, JSON_KEY_ICON);
    }
    else
    {
        pb_device_t* new_device = calloc(1, sizeof(pb_device_t));

        if ( strcmp(device_type, PHONE_ICON) == 0 )
        {
            new_device->type_device = ICON_PHONE;
            json_object_foreach_member(node_obj, _get_phone_device, &new_device->phone);
        }
        else if ( strcmp(device_type, BROWSER_ICON) == 0 )
        {
            new_device->type_device = ICON_BROWSER;
            json_object_foreach_member(node_obj, _get_browser_device, &new_device->browser);
        }

        // Add the device to the linked list
        if ( devices_list == NULL )
        {
            devices_list = new_device;
        }
        else
        {
            pb_device_t     *device = NULL;

            for ( device = devices_list; device->next != NULL; device = device->next )
            {
                ;
            }

            device->next = new_device;
        }
    }
}