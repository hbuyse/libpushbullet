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
#include "pb_devices_priv.h"
#include "pushbullet.h"


/**
 * @brief      Fill the browser structure with the informations contained in the JSON object
 *
 * @param[out]  browser  The browser structure we fill
 * @param[in]  json_obj  The JSON object
 */
static void devices_get_browser_device(JsonObject *object __attribute__((unused)),
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
static void devices_get_phone_device(JsonObject *object __attribute__((unused)),
                                     const gchar *member_name,
                                     JsonNode *member_node,
                                     gpointer userdata
                                     );


#ifdef __TRACES__
/**
 * @brief      Display on stdout the informations about the phone
 *
 * @param[in]  phone  The phone
 */
static void devices_dump_phone_infos(const pb_phone_t phone);


/**
 * @brief      Display on stdout the informations about the browser
 *
 * @param[in]  browser  The browser
 */
static void devices_dump_browser_infos(const pb_browser_t browser);


/**
 * @brief      Display all the devices of a given user
 *
 * @param      user  The user
 */
static void devices_dump_devices_list(const pb_devices_t *p_devices);
#endif


static void devices_fill_devices_list(JsonArray *arr __attribute__((unused)),
                                      guint idx,
                                      JsonNode *node_arr,
                                      gpointer userdata);


pb_device_t* pb_device_new()
{
    pb_device_t* d = calloc(1, sizeof(*d));

    if (d)
    {
        // Increase the reference
        d->ref++;
    }

    return d;
}


int pb_device_ref(pb_device_t* p_device)
{
    if (!p_device)
    {
        return -1;
    }

    p_device->ref++;
    return 0;
}


int pb_device_unref(pb_device_t* p_device)
{
    if (!p_device)
    {
        return -1;
    }

    if (--p_device->ref <= 0)
    {
        switch(p_device->type)
        {
            case ICON_PHONE:
                pb_free(p_device->phone.iden);
                pb_free(p_device->phone.nickname);
                pb_free(p_device->phone.manufacturer);
                pb_free(p_device->phone.model);
                pb_free(p_device->phone.fingerprint);
                pb_free(p_device->phone.push_token);
                pb_free(p_device->phone.icon);
                pb_free(p_device->phone.remote_files);
                break;

            case ICON_BROWSER:
                pb_free(p_device->browser.iden);
                pb_free(p_device->browser.nickname);
                pb_free(p_device->browser.manufacturer);
                pb_free(p_device->browser.model);
                pb_free(p_device->browser.icon);
                break;

            default:
                break;
        }

        free(p_device);
    }

    return 0;
}


pb_devices_t* pb_devices_new()
{
    pb_devices_t* d = calloc(1, sizeof(*d));

    if (d)
    {
        // Increase the reference
        d->ref++;
    }

    return d;
}


int pb_devices_ref(pb_devices_t* p_devices)
{
    if (!p_devices)
    {
        return -1;
    }

    p_devices->ref++;
    return 0;
}


int pb_devices_unref(pb_devices_t* p_devices)
{
    pb_device_t* list = NULL;

    if (!p_devices)
    {
        return -1;
    }

    if (--p_devices->ref <= 0)
    {
        // Pass through the list
        for ( list = p_devices->list; list != NULL; )
        {
            // Get the pointer
            pb_device_t* current_device = list;


            // Pass to the next pointer
            list   = list->next;


            // Free the temporary pointer
            pb_device_unref(current_device);
            p_devices->nb--;
        }

        free(p_devices);
    }

    return 0;
}


int pb_devices_load_devices_from_data(pb_devices_t* p_devices, char* result, size_t result_sz)
{
    int ret = -1;

    // JSON variables
    JsonParser *parser = NULL;
    JsonNode *root = NULL;
    JsonNode *devices_node = NULL;
    JsonArray *devices_arr = NULL;
    JsonObject *obj = NULL;

    if ( (! p_devices) || (! result) || (result_sz <= 0) )
    {
        return -1;
    }
    else
    {
        parser = json_parser_new();

        if ( json_parser_load_from_data(parser, result, result_sz, NULL) )
        {
            root = json_parser_get_root(parser);
            obj = json_node_get_object(root);

            if ( json_object_has_member(obj, DEVICES_JSON_KEY) )
            {
                devices_node = json_object_get_member(obj, DEVICES_JSON_KEY);
                devices_arr = json_node_get_array(devices_node);

                json_array_foreach_element(devices_arr, devices_fill_devices_list, p_devices);

                #ifdef __TRACES__
                iprintf("Nb of devices active: %zu", p_devices->nb);
                devices_dump_devices_list(p_devices);
                #endif
            }
            ret = 0;
        }
        else
        {
            ret = -1;
        }

        g_object_unref(parser);
    }


    return ret;
}


pb_device_t* pb_devices_get_list(const pb_devices_t* p_devices)
{
    return (p_devices) ? p_devices->list : NULL;
}


ssize_t pb_devices_get_number_active(const pb_devices_t *p_devices)
{
    return (p_devices) ? p_devices->nb : -1;
}


int pb_devices_add_new_device(pb_devices_t* p_devices, pb_device_t* p_new_device)
{
    pb_device_t     *device = NULL;

    if ( (! p_devices) || (! p_new_device))
    {
        return -1;
    }

    // Add the device to the linked list
    if ( p_devices->list == NULL )
    {
        p_devices->list = p_new_device;
    }
    else
    {
        for ( device = p_devices->list; device->next != NULL; device = device->next )
        {
            ;
        }

        device->next = p_new_device;
    }

    p_devices->nb++;

    return 0;
}


const char* pb_devices_get_iden_from_name(const pb_devices_t *p_devices,
                                          const char         *nickname
                                          )
{
    pb_device_t     *node = NULL;


    if ( ! nickname )
    {
        return (NULL);
    }

    for ( node = p_devices->list; node != NULL; node = node->next )
    {
        switch ( node->type )
        {
            case ICON_PHONE:

                if ( strcmp(node->phone.nickname, nickname) == 0 )
                {
                    return (node->phone.iden);
                }

                break;

            case ICON_BROWSER:

                if ( strcmp(node->browser.nickname, nickname) == 0 )
                {
                    return (node->browser.iden);
                }

                break;

            default:
                #ifdef __TRACES__
                eprintf("Unknown type...");
                #endif
                break;
        }
    }

    return (NULL);
}


static void devices_get_browser_device(JsonObject *object __attribute__((unused)),
                                       const gchar *member_name,
                                       JsonNode *member_node,
                                       gpointer userdata
                                       )
{
    pb_browser_t        *browser = (pb_browser_t*) userdata;

    if (JSON_NODE_HOLDS_VALUE(member_node))
    {
        JSON_ASSOCIATE_BOOL(browser, active);
        JSON_ASSOCIATE_STR(browser, iden);
        JSON_ASSOCIATE_DOUBLE(browser, created);
        JSON_ASSOCIATE_DOUBLE(browser, modified);
        JSON_ASSOCIATE_STR(browser, nickname);
        JSON_ASSOCIATE_STR(browser, manufacturer);
        JSON_ASSOCIATE_STR(browser, model);
        JSON_ASSOCIATE_INT(browser, app_version);
        JSON_ASSOCIATE_STR(browser, icon);
    }
}


static void devices_get_phone_device(JsonObject *object __attribute__((unused)),
                                     const gchar *member_name,
                                     JsonNode *member_node,
                                     gpointer userdata
                                     )
{
    pb_phone_t        *phone = (pb_phone_t*) userdata;

    if (JSON_NODE_HOLDS_VALUE(member_node))
    {
        JSON_ASSOCIATE_BOOL(phone, active);
        JSON_ASSOCIATE_STR(phone, iden);
        JSON_ASSOCIATE_DOUBLE(phone, created);
        JSON_ASSOCIATE_DOUBLE(phone, modified);
        JSON_ASSOCIATE_STR(phone, nickname);
        JSON_ASSOCIATE_BOOL(phone, generated_nickname);
        JSON_ASSOCIATE_STR(phone, manufacturer);
        JSON_ASSOCIATE_STR(phone, model);
        JSON_ASSOCIATE_INT(phone, app_version);
        JSON_ASSOCIATE_STR(phone, push_token);
        JSON_ASSOCIATE_BOOL(phone, has_sms);
        JSON_ASSOCIATE_BOOL(phone, has_mms);
        JSON_ASSOCIATE_STR(phone, icon);
        JSON_ASSOCIATE_STR(phone, remote_files);
        JSON_ASSOCIATE_STR(phone, fingerprint);       // JsonObject
    }
}


#ifdef __TRACES__
static void devices_dump_phone_infos(const pb_phone_t phone)
{
    iprintf("%c%s - %s", phone.icon[0] - 32, phone.icon + 1, phone.iden);
    iprintf("\tactive : %u", phone.active);
    iprintf("\tcreated : %f", phone.created);
    iprintf("\tmodified : %f", phone.modified);
    iprintf("\tnickname : %s", phone.nickname);
    iprintf("\tgenerated_nickname : %u", phone.generated_nickname);
    iprintf("\tmanufacturer : %s", phone.manufacturer);
    iprintf("\tmodel : %s", phone.model);
    iprintf("\tapp_version : %hd", phone.app_version);
    iprintf("\tfingerprint : %s", phone.fingerprint);
    iprintf("\tpush_token : %s", phone.push_token);
    iprintf("\thas_sms : %u", phone.has_sms);
    iprintf("\thas_mms : %u", phone.has_mms);
    iprintf("\tremote_files : %s", phone.remote_files);
}


static void devices_dump_browser_infos(const pb_browser_t browser)
{
    iprintf("%c%s - %s", browser.icon[0] - 32, browser.icon + 1, browser.iden);
    iprintf("\tactive : %u", browser.active);
    iprintf("\tcreated : %f", browser.created);
    iprintf("\tmodified : %f", browser.modified);
    iprintf("\tnickname : %s", browser.nickname);
    iprintf("\tmanufacturer : %s", browser.manufacturer);
    iprintf("\tmodel : %s", browser.model);
    iprintf("\tapp_version : %hd", browser.app_version);
}


static void devices_dump_devices_list(const pb_devices_t *p_devices)
{
    pb_device_t     *node = NULL;


    for ( node = p_devices->list; node != NULL; node = node->next )
    {
        switch ( node->type )
        {
            case ICON_PHONE:
                devices_dump_phone_infos(node->phone);
                break;

            case ICON_BROWSER:
                devices_dump_browser_infos(node->browser);
                break;

            default:
                eprintf("Unknown type...");
        }
    }
}
#endif


static void devices_fill_devices_list(JsonArray *arr __attribute__((unused)),
                                      guint idx,
                                      JsonNode *node_arr,
                                      gpointer userdata)
{
    pb_devices_t* p_devices = (pb_devices_t*) userdata;
    JsonObject* node_obj = NULL;
    const char* device_type = NULL;

    if ( ! JSON_NODE_HOLDS_OBJECT(node_arr) )
    {
        eprintf("devices[%d] : The node does not contain an JsonObject", idx);
    }
    else if ( (node_obj = json_node_get_object(node_arr) ) == NULL)
    {
        eprintf("devices[%d] : Impossible to get the object from the node", idx);
    }
    else if ( ! json_object_has_member(node_obj, ACTIVE_JSON_KEY) )
    {
        eprintf("devices[%d] : The obj does not have the member \"%s\"", idx, ACTIVE_JSON_KEY);
    }
    else if ( ! json_object_get_boolean_member(node_obj, ACTIVE_JSON_KEY) )
    {
        // Active: false
        return;
    }
    else if ( ! json_object_has_member(node_obj, JSON_KEY_ICON) )
    {
        eprintf("devices[%d] : The obj does not have the member \"%s\"", idx, JSON_KEY_ICON);
    }
    else if ( (device_type = json_object_get_string_member(node_obj, JSON_KEY_ICON)) == NULL )
    {
        eprintf("devices[%d] : Impossible to get the string member \"%s\" from object", idx, JSON_KEY_ICON);
    }
    else
    {
        pb_device_t* new_device = pb_device_new();

        if ( strcmp(device_type, PHONE_ICON) == 0 )
        {
            new_device->type = ICON_PHONE;
            json_object_foreach_member(node_obj, devices_get_phone_device, &new_device->phone);
        }
        else if ( strcmp(device_type, BROWSER_ICON) == 0 )
        {
            new_device->type = ICON_BROWSER;
            json_object_foreach_member(node_obj, devices_get_browser_device, &new_device->browser);
        }

        pb_devices_add_new_device(p_devices, new_device);
    }
}