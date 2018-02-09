/**
 * @file pb_device.c
 * @author hbuyse
 * @date 09/02/2018
 */
#include <stdlib.h>          // free
#include <string.h>          // strcmp
#include <json-glib/json-glib.h>          // JsonObject, json_tokener_parse, json_object_object_foreach, json_object_get_array,
                                // array_list

#include "pb_utils.h"             // iprintf, eprintf, cprintf, gprintf
#include "pb_device_priv.h"


pb_device_t* pb_device_new(void)
{
    pb_device_t* d = calloc(1, sizeof(*d));

    if ( d )
    {
        // Increase the reference
        d->ref++;

        // Unknown type
        d->type = ICON_UNKNOWN;
    }

    return d;
}


int pb_device_get_ref(pb_device_t* p_device)
{
    return ( p_device ) ? p_device->ref : 0;
}


int pb_device_ref(pb_device_t* p_device)
{
    if ( ! p_device )
    {
        return -1;
    }

    p_device->ref++;
    return 0;
}


int pb_device_unref(pb_device_t* p_device)
{
    if ( ! p_device )
    {
        return -1;
    }

    if ( --p_device->ref <= 0 )
    {
        switch( p_device->type )
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


int pb_device_set_type(pb_device_t* p_device, pb_device_icon type)
{
    if ( ! p_device )
    {
        return -1;
    }
    
    p_device->type = type;

    return 0;
}


pb_device_icon pb_device_get_type(pb_device_t* p_device)
{
    return ( p_device ) ? p_device->type : ICON_UNKNOWN;
}


int pb_device_set_next(pb_device_t* p_device, pb_device_t* p_next)
{
    if ( ! p_device )
    {
        return -1;
    }
    
    p_device->next = p_next;

    return 0;
}


pb_device_t* pb_device_get_next(pb_device_t* p_device)
{
    return ( p_device ) ? p_device->next : NULL;
}


unsigned char pb_device_is_active(pb_device_t* p_device)
{
    unsigned char ret = 0;

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.active;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.active;
                break;

            default:
                break;
        }
    }

    return ret;
}


char* pb_device_get_iden(const pb_device_t* p_device)
{
    char* ret = NULL;

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.iden;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.iden;
                break;

            default:
                break;
        }
    }

    return ret;
}


double pb_device_get_created(const pb_device_t* p_device)
{
    double ret = 0;    

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.created;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.created;
                break;

            default:
                break;
        }
    }

    return ret;
}


double pb_device_get_modified(const pb_device_t* p_device)
{
    double ret = 0;    

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.modified;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.modified;
                break;

            default:
                break;
        }
    }

    return ret;
}


char* pb_device_get_nickname(const pb_device_t* p_device)
{
    char* ret = 0;    

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.nickname;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.nickname;
                break;

            default:
                break;
        }
    }

    return ret;
}


char* pb_device_get_manufacturer(const pb_device_t* p_device)
{
    char* ret = 0;    

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.manufacturer;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.manufacturer;
                break;

            default:
                break;
        }
    }

    return ret;
}


char* pb_device_get_model(const pb_device_t* p_device)
{
    char* ret = 0;    

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.model;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.model;
                break;

            default:
                break;
        }
    }

    return ret;
}


short pb_device_get_app_version(const pb_device_t* p_device)
{
    short  ret = 0;    

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.app_version;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.app_version;
                break;

            default:
                break;
        }
    }

    return ret;
}


char* pb_device_get_icon(const pb_device_t* p_device)
{
    char* ret = 0;    

    if ( p_device )
    {
        switch( p_device->type )
        {
            case ICON_PHONE:
                ret = p_device->phone.icon;
                break;

            case ICON_BROWSER:
                ret = p_device->browser.icon;
                break;

            default:
                break;
        }
    }

    return ret;
}


void pb_device_fill_from_json(JsonObject *object __attribute__((unused)),
                              const gchar *member_name,
                              JsonNode *member_node,
                              gpointer userdata
                              )
{
    pb_device_t *p_device = (pb_device_t*) userdata;

    if ( ! p_device )
    {
        return;
    }
    else if ( ! JSON_NODE_HOLDS_VALUE(member_node) )
    {
        return;
    }
    else
    {
        switch(p_device->type)
        {
            case ICON_BROWSER:
                JSON_ASSOCIATE_BOOL(p_device->browser, active);
                JSON_ASSOCIATE_STR(p_device->browser, iden);
                JSON_ASSOCIATE_DOUBLE(p_device->browser, created);
                JSON_ASSOCIATE_DOUBLE(p_device->browser, modified);
                JSON_ASSOCIATE_STR(p_device->browser, nickname);
                JSON_ASSOCIATE_STR(p_device->browser, manufacturer);
                JSON_ASSOCIATE_STR(p_device->browser, model);
                JSON_ASSOCIATE_INT(p_device->browser, app_version);
                JSON_ASSOCIATE_STR(p_device->browser, icon);
                break;
            case ICON_PHONE:
                JSON_ASSOCIATE_BOOL(p_device->phone, active);
                JSON_ASSOCIATE_STR(p_device->phone, iden);
                JSON_ASSOCIATE_DOUBLE(p_device->phone, created);
                JSON_ASSOCIATE_DOUBLE(p_device->phone, modified);
                JSON_ASSOCIATE_STR(p_device->phone, nickname);
                JSON_ASSOCIATE_BOOL(p_device->phone, generated_nickname);
                JSON_ASSOCIATE_STR(p_device->phone, manufacturer);
                JSON_ASSOCIATE_STR(p_device->phone, model);
                JSON_ASSOCIATE_INT(p_device->phone, app_version);
                JSON_ASSOCIATE_STR(p_device->phone, push_token);
                JSON_ASSOCIATE_BOOL(p_device->phone, has_sms);
                JSON_ASSOCIATE_BOOL(p_device->phone, has_mms);
                JSON_ASSOCIATE_STR(p_device->phone, icon);
                JSON_ASSOCIATE_STR(p_device->phone, remote_files);
                JSON_ASSOCIATE_STR(p_device->phone, fingerprint);       // JsonObject
                break;
            default:
                break;
        }
    }
}


#ifdef __TRACES__
void pb_device_dump_infos(const pb_device_t* p_device)
{
    switch ( p_device->type )
    {
        case ICON_BROWSER:
            iprintf("%c%s - %s", p_device->browser.icon[0] - 32, p_device->browser.icon + 1, p_device->browser.iden);
            iprintf("\tactive : %u", p_device->browser.active);
            iprintf("\tcreated : %f", p_device->browser.created);
            iprintf("\tmodified : %f", p_device->browser.modified);
            iprintf("\tnickname : %s", p_device->browser.nickname);
            iprintf("\tmanufacturer : %s", p_device->browser.manufacturer);
            iprintf("\tmodel : %s", p_device->browser.model);
            iprintf("\tapp_version : %hd", p_device->browser.app_version);
            break;

        case ICON_PHONE:
            iprintf("%c%s - %s", p_device->phone.icon[0] - 32, p_device->phone.icon + 1, p_device->phone.iden);
            iprintf("\tactive : %u", p_device->phone.active);
            iprintf("\tcreated : %f", p_device->phone.created);
            iprintf("\tmodified : %f", p_device->phone.modified);
            iprintf("\tnickname : %s", p_device->phone.nickname);
            iprintf("\tgenerated_nickname : %u", p_device->phone.generated_nickname);
            iprintf("\tmanufacturer : %s", p_device->phone.manufacturer);
            iprintf("\tmodel : %s", p_device->phone.model);
            iprintf("\tapp_version : %hd", p_device->phone.app_version);
            iprintf("\tfingerprint : %s", p_device->phone.fingerprint);
            iprintf("\tpush_token : %s", p_device->phone.push_token);
            iprintf("\thas_sms : %u", p_device->phone.has_sms);
            iprintf("\thas_mms : %u", p_device->phone.has_mms);
            iprintf("\tremote_files : %s", p_device->phone.remote_files);
            break;

        default:
            eprintf("Unknown type...");
            break;
    }
}
#endif
