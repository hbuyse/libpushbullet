/**
 * @file devices.c
 * @author hbuyse
 * @date 08/05/2016
 */

#include <string.h>          // strcmpt
#include <json/json.h>          // json_object, json_tokener_parse, json_object_object_foreach, json_object_get_array,
                                // array_list

#include <pushbullet/urls.h>          // API_URL_DEVICES
#include <pushbullet/requests.h>          // pb_get
#include <pushbullet/user.h>          // pb_user_t
#include <pushbullet/structures.h>          // pb_browser_t, pb_phone_t, pb_device_t, ICON_PHONE, ICON_BROWSER
#include <pushbullet/http_code.h>          // HTTP_OK
#include <pushbullet/logging.h>             // iprintf, eprintf, cprintf, gprintf


/**
 * @def MAX_SIZE_BUF
 * Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000


/**
 * @def DEVICES_JSON_KEY
 * String defining the "devices" key
 */
#define DEVICES_JSON_KEY "devices"


/**
 * @def ACTIVE_JSON_KEY
 * String defining the "active" key
 */
#define ACTIVE_JSON_KEY "active"


/**
 * @def JSON_KEY_ICON
 * String defining the "icon" key
 */
#define JSON_KEY_ICON "icon"


/**
 * @def DESKTOP_ICON
 * String defining the "desktop" key
 */
#define DESKTOP_ICON "desktop"


/**
 * @def BROWSER_ICON
 * String defining the "browser" key
 */
#define BROWSER_ICON "browser"


/**
 * @def WEBSITE_ICON
 * String defining the "website" key
 */
#define WEBSITE_ICON "website"


/**
 * @def LAPTOP_ICON
 * String defining the "laptop" key
 */
#define LAPTOP_ICON "laptop"


/**
 * @def TABLET_ICON
 * String defining the "tablet" key
 */
#define TABLET_ICON "tablet"


/**
 * @def PHONE_ICON
 * String defining the "phone" key
 */
#define PHONE_ICON "phone"


/**
 * @def WATCH_ICON
 * String defining the "watch" key
 */
#define WATCH_ICON "watch"


/**
 * @def SYSTEM_ICON
 * String defining the "system" key
 */
#define SYSTEM_ICON "system"


/**
 * @def      JSON_ASSOCIATE(type, var, k)
 * Macro to associate a key in a structure
 */
#define     JSON_ASSOCIATE(type, var, k)          \
    if ( strcmp(key, # k) == 0 ) {var->k = json_object_get_ ## type(val); }


/**
 * @brief      Fill the browser structure with the informations contained in the JSON object
 *
 * @param[out]  browser  The browser structure we fill
 * @param[in]  json_obj  The JSON object
 */
static void _get_browser_device(pb_browser_t        *browser,
                                const json_object   *json_obj
                                )
{
    json_object_object_foreach( (json_object *) json_obj, key, val)
    {
        JSON_ASSOCIATE(boolean, browser, active)
        JSON_ASSOCIATE(string, browser, iden)
        JSON_ASSOCIATE(double, browser, created)
        JSON_ASSOCIATE(double, browser, modified)
        JSON_ASSOCIATE(string, browser, nickname)
        JSON_ASSOCIATE(string, browser, manufacturer)
        JSON_ASSOCIATE(string, browser, model)
        JSON_ASSOCIATE(int, browser, app_version)
        JSON_ASSOCIATE(string, browser, icon)


        // JSON_ASSOCIATE(boolean, browser, fingerprint)       // json_object
    }
}



/**
 * @brief      Fill the phone structure with the informations contained in the JSON object
 *
 * @param[out]  phone  The phone structure we fill
 * @param[in]  json_obj  The JSON object
 */
static void _get_phone_device(pb_phone_t        *phone,
                              const json_object *json_obj
                              )
{
    json_object_object_foreach( (json_object *) json_obj, key, val)
    {
        JSON_ASSOCIATE(boolean, phone, active)
        JSON_ASSOCIATE(string, phone, iden)
        JSON_ASSOCIATE(double, phone, created)
        JSON_ASSOCIATE(double, phone, modified)
        JSON_ASSOCIATE(string, phone, nickname)
        JSON_ASSOCIATE(boolean, phone, generated_nickname)
        JSON_ASSOCIATE(string, phone, manufacturer)
        JSON_ASSOCIATE(string, phone, model)
        JSON_ASSOCIATE(int, phone, app_version)
        JSON_ASSOCIATE(string, phone, push_token)
        JSON_ASSOCIATE(boolean, phone, has_sms)
        JSON_ASSOCIATE(boolean, phone, has_mms)
        JSON_ASSOCIATE(string, phone, icon)
        JSON_ASSOCIATE(string, phone, remote_files)


        // JSON_ASSOCIATE(boolean, phone, fingerprint)       // json_object
    }
}



/**
 * @brief      Display on stdout the informations about the phone
 *
 * @param[in]  phone  The phone
 */
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
    iprintf("\tfingerprint : %p\n", phone.fingerprint);
    iprintf("\tpush_token : %s\n", phone.push_token);
    iprintf("\thas_sms : %u\n", phone.has_sms);
    iprintf("\thas_mms : %u\n", phone.has_mms);
    iprintf("\tremote_files : %s\n", phone.remote_files);
}



/**
 * @brief      Display on stdout the informations about the browser
 *
 * @param[in]  browser  The browser
 */
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



/**
 * @brief      Display all the devices of a given user
 *
 * @param      user  The user
 */
static void _dump_devices_list(pb_user_t *user)
{
    pb_device_t     *tmp = NULL;


    for ( tmp = user->devices; tmp != NULL; tmp = tmp->next )
    {
        switch ( tmp->type_device )
        {
            case ICON_PHONE:
                _dump_phone_infos(tmp->phone);
                break;

            case ICON_BROWSER:
                _dump_browser_infos(tmp->browser);
                break;

            default:
                eprintf("Unknown type...\n");
        }
    }
}



unsigned short pb_get_devices(pb_user_t *user)
{
    unsigned char       i                   = 0;


    // cUrl results
    char                *result             = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    unsigned short      res                 = 0;


    // JSON variables
    json_object         *json_obj           = NULL;
    json_object         *json_devices       = NULL;
    json_object         *json_value         = NULL;
    int                 json_devices_len    = 0;


    res = pb_get(result, API_URL_DEVICES, *user);

    #ifdef __DEBUG__
    if ( res == HTTP_OK )
    {
        gprintf("\e[37m %u\e[0m %s\n", res, result);
    }
    else
    {
        eprintf("\e[37m %u\e[0m %s\n", res, result);
    }
    #endif


    // If we do not have a 200 OK, we stop the function and we return the HTTP Status code
    if ( res != HTTP_OK )
    {
        return (res);
    }


    // Parse the result
    json_obj            = json_tokener_parse(result);


    // Get the list of devices at the key "devices" and get its length
    json_object_object_get_ex(json_obj, DEVICES_JSON_KEY, &json_devices);
    json_devices_len    = json_object_array_length(json_devices);

    #ifdef __DEBUG__
    iprintf("json_devices_len : %d\n", json_devices_len);
    #endif


    // Parse all the devices in the list
    for ( i = 0; i < json_devices_len; ++i )
    {
        json_object     *json_active = NULL;


        // Getting the array element at position i
        json_value = json_object_array_get_idx(json_devices, i);


        // Get the value of the key "active" of the json_object
        json_object_object_get_ex(json_value, ACTIVE_JSON_KEY, &json_active);


        // Pass this increment if the device is not active
        if ( json_object_get_boolean(json_active) != 1 )
        {
            continue;
        }


        // Declaration of the needed variables
        json_object     *json_kind  = NULL;
        pb_device_t     *device     = (pb_device_t *) calloc(1, sizeof(pb_device_t) );


        // Get the kind of PushBullet device
        json_object_object_get_ex(json_value, JSON_KEY_ICON, &json_kind);

        if ( strcmp(json_object_get_string(json_kind), PHONE_ICON) == 0 )
        {
            device->type_device = ICON_PHONE;
            _get_phone_device(&(device->phone), json_value);
        }
        else if ( strcmp(json_object_get_string(json_kind), BROWSER_ICON) == 0 )
        {
            device->type_device = ICON_BROWSER;
            _get_browser_device(&(device->browser), json_value);
        }


        // Add the device to the linked list
        if ( user->devices == NULL )
        {
            user->devices = device;
        }
        else
        {
            pb_device_t     *tmp = NULL;

            for ( tmp = user->devices; tmp->next != NULL; tmp = tmp->next )
            {
                ;
            }

            tmp->next = device;
        }
    }

    #ifdef __DEBUG__
    _dump_devices_list(user);
    #endif

    if ( result )
    {
        free(result);
    }

    return (res);
}



void pb_free_devices(pb_user_t *user)
{
    pb_device_t     *tmp = NULL;


    // Pass through the list
    for ( tmp = user->devices; user->devices != NULL; )
    {
        // Get the pointer
        tmp             = user->devices;


        // Pass to the next pointer
        user->devices   = user->devices->next;


        // Free the temporary pointer
        free(tmp);
    }

    #ifdef __DEBUG__
    _dump_devices_list(user);
    #endif
}



unsigned char pb_get_number_active_devices(pb_user_t user)
{
    unsigned char       i       = 0;
    pb_device_t         *tmp    = NULL;


    for ( tmp = user.devices, i = 0; tmp != NULL; tmp = tmp->next, ++i )
    {
        ;
    }

    return (i);
}



const char* pb_get_iden_from_name(const pb_user_t   user,
                                  const char        *name
                                  )
{
    pb_device_t     *tmp = NULL;


    if ( ! name )
    {
        return (NULL);
    }

    for ( tmp = user.devices; tmp != NULL; tmp = tmp->next )
    {
        switch ( tmp->type_device )
        {
            case ICON_PHONE:

                if ( strcmp(tmp->phone.nickname, name) == 0 )
                {
                    return (tmp->phone.iden);
                }

                break;

            case ICON_BROWSER:

                if ( strcmp(tmp->browser.nickname, name) == 0 )
                {
                    return (tmp->browser.iden);
                }

                break;

            default:
                eprintf("Unknown type...\n");
        }
    }

    return (NULL);
}