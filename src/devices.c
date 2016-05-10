/**
 * @file devices.c
 * @date 08/05/2016
 */

#include <stdio.h>          // fprintf, stdout
#include <string.h>          // strcmpt

#include <requests.h>          // pb_get, API_URL_DEVICES
#include <user.h>          // PB_user_t
#include <devices.h>          // PB_browser_t
#include <json/json.h>          // json_object, json_tokener_parse, json_object_object_foreach, json_object_get_array,
                                // array_list
#include <http_code.h>          // HTTP_OK


/**
 * \brief Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000

#define DEVICES_JSON_KEY "devices"
#define ACTIVE_JSON_KEY "active"
#define KIND_JSON_KEY "kind"


#define ANDROID_KIND "android"
#define CHROME_KIND "chrome"
#define IPHONE_KIND "iphone"


/**
 * \brief      Macro to associate a key in a structure
 *
 * \param      type  The type
 * \param      var   The pointer we fill
 * \param      k     The JSON key
 */
#define     JSON_ASSOCIATE(type, var, k)          \
    if ( strcmp(key, # k) == 0 ) {var->k = json_object_get_ ## type(val); }


/**
 * \brief      Fill the browser structure with the informations contained in the JSON object
 *
 * \param[out]  browser  The browser structure we fill
 * \param[in]  json_obj  The JSON object
 */
static void _get_browser_device(PB_browser_t    *browser,
                                json_object     *json_obj
                                )
{
    json_object_object_foreach(json_obj, key, val)
    {
        JSON_ASSOCIATE(boolean, browser, active)
        JSON_ASSOCIATE(string, browser, iden)
        JSON_ASSOCIATE(double, browser, created)
        JSON_ASSOCIATE(double, browser, modified)
        JSON_ASSOCIATE(string, browser, type)
        JSON_ASSOCIATE(string, browser, kind)
        JSON_ASSOCIATE(string, browser, nickname)
        JSON_ASSOCIATE(string, browser, manufacturer)
        JSON_ASSOCIATE(string, browser, model)
        JSON_ASSOCIATE(int, browser, app_version)
        JSON_ASSOCIATE(boolean, browser, pushable)
        JSON_ASSOCIATE(string, browser, icon)


        // JSON_ASSOCIATE(boolean, browser, fingerprint)       // json_object
    }
}



/**
 * \brief      Fill the phone structure with the informations contained in the JSON object
 *
 * \param[out]  phone  The phone structure we fill
 * \param[in]  json_obj  The JSON object
 */
static void _get_phone_device(PB_phone_t    *phone,
                              json_object   *json_obj
                              )
{
    json_object_object_foreach(json_obj, key, val)
    {
        JSON_ASSOCIATE(boolean, phone, active)
        JSON_ASSOCIATE(string, phone, iden)
        JSON_ASSOCIATE(double, phone, created)
        JSON_ASSOCIATE(double, phone, modified)
        JSON_ASSOCIATE(string, phone, type)
        JSON_ASSOCIATE(string, phone, kind)
        JSON_ASSOCIATE(string, phone, nickname)
        JSON_ASSOCIATE(boolean, phone, generated_nickname)
        JSON_ASSOCIATE(string, phone, manufacturer)
        JSON_ASSOCIATE(string, phone, model)
        JSON_ASSOCIATE(int, phone, app_version)
        JSON_ASSOCIATE(string, phone, push_token)
        JSON_ASSOCIATE(boolean, phone, has_sms)
        JSON_ASSOCIATE(boolean, phone, has_mms)
        JSON_ASSOCIATE(boolean, phone, pushable)
        JSON_ASSOCIATE(string, phone, icon)
        JSON_ASSOCIATE(string, phone, remote_files)


        // JSON_ASSOCIATE(boolean, phone, fingerprint)       // json_object
    }
}



/**
 * \brief      Display on stdout the informations about the phone
 *
 * \param[in]  phone  The phone
 */
static void _dump_phone_infos(PB_phone_t phone)
{
    fprintf(stdout, "\e[1m[%s]\e[0m %c%s - %s\n", __func__, phone.kind[0] - 32, phone.kind + 1, phone.iden);
    fprintf(stdout, "\e[1m[%s]\e[0m\tactive : %u\n", __func__, phone.active);
    fprintf(stdout, "\e[1m[%s]\e[0m\tcreated : %f\n", __func__, phone.created);
    fprintf(stdout, "\e[1m[%s]\e[0m\tmodified : %f\n", __func__, phone.modified);
    fprintf(stdout, "\e[1m[%s]\e[0m\ttype : %s\n", __func__, phone.type);
    fprintf(stdout, "\e[1m[%s]\e[0m\tnickname : %s\n", __func__, phone.nickname);
    fprintf(stdout, "\e[1m[%s]\e[0m\tgenerated_nickname : %u\n", __func__, phone.generated_nickname);
    fprintf(stdout, "\e[1m[%s]\e[0m\tmanufacturer : %s\n", __func__, phone.manufacturer);
    fprintf(stdout, "\e[1m[%s]\e[0m\tmodel : %s\n", __func__, phone.model);
    fprintf(stdout, "\e[1m[%s]\e[0m\tapp_version : %hd\n", __func__, phone.app_version);
    fprintf(stdout, "\e[1m[%s]\e[0m\tfingerprint : %p\n", __func__, phone.fingerprint);
    fprintf(stdout, "\e[1m[%s]\e[0m\tpush_token : %s\n", __func__, phone.push_token);
    fprintf(stdout, "\e[1m[%s]\e[0m\tpushable : %u\n", __func__, phone.pushable);
    fprintf(stdout, "\e[1m[%s]\e[0m\thas_sms : %u\n", __func__, phone.has_sms);
    fprintf(stdout, "\e[1m[%s]\e[0m\thas_mms : %u\n", __func__, phone.has_mms);
    fprintf(stdout, "\e[1m[%s]\e[0m\ticon : %s\n", __func__, phone.icon);
    fprintf(stdout, "\e[1m[%s]\e[0m\tremote_files : %s\n", __func__, phone.remote_files);
}



/**
 * \brief      Display on stdout the informations about the browser
 *
 * \param[in]  browser  The browser
 */
static void _dump_browser_infos(PB_browser_t browser)
{
    fprintf(stdout, "\e[1m[%s]\e[0m %c%s - %s\n", __func__, browser.kind[0] - 32, browser.kind + 1, browser.iden);
    fprintf(stdout, "\e[1m[%s]\e[0m\tactive : %u\n", __func__, browser.active);
    fprintf(stdout, "\e[1m[%s]\e[0m\tcreated : %f\n", __func__, browser.created);
    fprintf(stdout, "\e[1m[%s]\e[0m\tmodified : %f\n", __func__, browser.modified);
    fprintf(stdout, "\e[1m[%s]\e[0m\ttype : %s\n", __func__, browser.type);
    fprintf(stdout, "\e[1m[%s]\e[0m\tnickname : %s\n", __func__, browser.nickname);
    fprintf(stdout, "\e[1m[%s]\e[0m\tmanufacturer : %s\n", __func__, browser.manufacturer);
    fprintf(stdout, "\e[1m[%s]\e[0m\tmodel : %s\n", __func__, browser.model);
    fprintf(stdout, "\e[1m[%s]\e[0m\tapp_version : %hd\n", __func__, browser.app_version);
    fprintf(stdout, "\e[1m[%s]\e[0m\tpushable : %u\n", __func__, browser.pushable);
    fprintf(stdout, "\e[1m[%s]\e[0m\ticon : %s\n", __func__, browser.icon);
}



unsigned short pb_get_devices(PB_user_t user)
{
    unsigned char       i                   = 0;


    // cUrl results
    char                *result             = (char *) calloc(MAX_SIZE_BUF, sizeof(char) );
    unsigned char       res                 = 0;


    // JSON variables
    json_object         *json_obj           = NULL;
    json_object         *json_devices       = NULL;
    json_object         *json_value         = NULL;
    int                 json_devices_len    = 0;


    res                 = pb_get(result, API_URL_DEVICES, user.token_key);

    #ifdef __DEBUG__
    fprintf( (res == HTTP_OK) ? stdout : stderr, "\e[1m[%s]\e[0m %s\n", __func__, result);
    #endif


    // Parse the result
    json_obj            = json_tokener_parse(result);


    // Get the list of devices at the key "devices" and get its length
    json_object_object_get_ex(json_obj, DEVICES_JSON_KEY, &json_devices);
    json_devices_len    = json_object_array_length(json_devices);

    #ifdef __DEBUG__
    fprintf(stdout, "\e[1m[%s]\e[0m json_devices_len : %d\n", __func__, json_devices_len);
    #endif


    // Parse all the devices in the list
    for ( i = 0; i < json_devices_len; ++i )
    {
        json_object     *json_active    = NULL;
        json_object     *json_kind      = NULL;


        // Getting the array element at position i
        json_value = json_object_array_get_idx(json_devices, i);


        // Get the value of the key "active" of the json_object
        json_object_object_get_ex(json_value, ACTIVE_JSON_KEY, &json_active);


        // Pass this increment since the device is not active
        if ( json_object_get_boolean(json_active) != 1 )
        {
            continue;
        }


        // Get the kind of PushBullet device
        json_object_object_get_ex(json_value, KIND_JSON_KEY, &json_kind);

        if ( strcmp(json_object_get_string(json_kind), ANDROID_KIND) == 0 )
        {
            PB_phone_t     *phone = (PB_phone_t *) calloc(1, sizeof(PB_phone_t) );
            _get_phone_device(phone, json_value);

            #ifdef __DEBUG__
            _dump_phone_infos(*phone);
            #endif
        }
        else if ( strcmp(json_object_get_string(json_kind), CHROME_KIND) == 0 )
        {
            PB_browser_t     *browser = (PB_browser_t *) calloc(1, sizeof(PB_browser_t) );
            _get_browser_device(browser, json_value);

            #ifdef __DEBUG__
            _dump_browser_infos(*browser);
            #endif
        }
    }

    if ( result )
    {
        free(result);
    }

    return (res);
}