/**
 * @file devices.c
 * @author hbuyse
 * @date 08/05/2016
 */
#include <stdlib.h>          // free
#include <string.h>          // strcmp
#include <json-glib/json-glib.h>          // JsonObject, json_tokener_parse, json_object_object_foreach, json_object_get_array,
                                // array_list

#include "pb_utils.h"             // iprintf, eprintf, cprintf, gprintf
#include "pb_devices_priv.h"
#include "pb_device_prot.h"     // pb_device_t, pb_device_new, pb_device_unref, pb_device_set_browser_device, pb_device_set_phone_device
#include "pushbullet.h"



static void devices_fill_devices_list(JsonArray *arr __attribute__((unused)),
                                      guint idx,
                                      JsonNode *node_arr,
                                      gpointer userdata
                                      );


#ifdef __TRACES__



/**
 * @brief      Display all the devices of a given user
 *
 * @param      user  The user
 */
static void devices_dump_devices_list(const pb_devices_t *p_devices);
#endif


pb_devices_t* pb_devices_new()
{
    pb_devices_t* d = calloc(1, sizeof(*d));

    if ( d )
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
            list   = pb_device_get_next(list);


            // Free the temporary pointer
            pb_device_unref(current_device);
            p_devices->nb_active--;
        }

        free(p_devices);
    }

    return 0;
}


int pb_devices_get_ref(const pb_devices_t* p_devices)
{
    return (p_devices) ? p_devices->ref : 0;
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
                iprintf("Nb of devices active: %zu", p_devices->nb_active);
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
    return (p_devices) ? p_devices->nb_active : -1;
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
        for ( device = p_devices->list; pb_device_get_next(device) != NULL; device = pb_device_get_next(device) )
        {
            ;
        }

        pb_device_set_next(device, p_new_device);
    }

    p_devices->nb_active++;

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

    for ( node = p_devices->list; node != NULL; node = pb_device_get_next(node) )
    {
        switch ( pb_device_get_type(node) )
        {
            case ICON_PHONE:

                if ( strcmp(pb_device_get_nickname(node), nickname) == 0 )
                {
                    return ( pb_device_get_iden(node) );
                }

                break;

            case ICON_BROWSER:

                if ( strcmp(pb_device_get_nickname(node), nickname) == 0 )
                {
                    return ( pb_device_get_iden(node) );
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
            pb_device_set_type(new_device, ICON_PHONE);
        }
        else if ( strcmp(device_type, BROWSER_ICON) == 0 )
        {
            pb_device_set_type(new_device, ICON_BROWSER);
        }
            
        json_object_foreach_member(node_obj, pb_device_fill_from_json, new_device);

        pb_devices_add_new_device(p_devices, new_device);
    }
}


#ifdef __TRACES__
static void devices_dump_devices_list(const pb_devices_t *p_devices)
{
    pb_device_t     *node = NULL;


    for ( node = p_devices->list; node != NULL; node = pb_device_get_next(node) )
    {
        pb_device_dump_infos(node);
    }
}
#endif
