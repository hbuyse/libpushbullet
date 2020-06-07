/**
 * @file pb_device.c
 * @author hbuyse
 * @date 09/02/2018
 */
#include <stdlib.h>          // free
#include <string.h>          // strcmp
#include <jansson.h>

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


pb_device_icon pb_device_get_type(const pb_device_t* p_device)
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


pb_device_t* pb_device_get_next(const pb_device_t* p_device)
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


void pb_device_fill_from_json(pb_device_t* p_device,
                              json_t *root
                              )
{
    if ( ! p_device )
    {
        return;
    }
    else
    {
        json_error_t err = {0};
        int ret = -1;
        int active = 0;
        json_t *val = NULL;

        if (! (val = json_object_get(root, "active")))
            eprintf("json_object_get: could not find the active key");
        else if (json_is_false(val))
            return;

        else {
            switch(p_device->type)
            {
                case ICON_BROWSER:
                    gprintf("browser");
                    ret = json_unpack_ex(root, &err, 0,
                        "{sbsssfsfsssssssiss}",
                        "active", &p_device->browser.active,
                        "iden", &p_device->browser.iden,
                        "created", &p_device->browser.created,
                        "modified", &p_device->browser.modified,
                        "nickname", &p_device->browser.nickname,
                        "manufacturer", &p_device->browser.manufacturer,
                        "model", &p_device->browser.model,
                        "app_version", &p_device->browser.app_version,
                        "icon", &p_device->browser.icon
                        );
                    break;
                case ICON_PHONE:
                    ret = json_unpack_ex(root, &err, 0,
                        "{sbsssfsfsssbsssssisssbsbssssss}",
                        "active", &p_device->phone.active,
                        "iden", &p_device->phone.iden,
                        "created", &p_device->phone.created,
                        "modified", &p_device->phone.modified,
                        "nickname", &p_device->phone.nickname,
                        "generated_nickname", &p_device->phone.generated_nickname,
                        "manufacturer", &p_device->phone.manufacturer,
                        "model", &p_device->phone.model,
                        "app_version", &p_device->phone.app_version,
                        "push_token", &p_device->phone.push_token,
                        "has_sms", &p_device->phone.has_sms,
                        "has_mms", &p_device->phone.has_mms,
                        "icon", &p_device->phone.icon,
                        "remote_files", &p_device->phone.remote_files,
                        "fingerprint", &p_device->phone.fingerprint
                    );
                    break;
                default:
                    break;
            }

            if (ret) {
                gprintf("%s", json_dumps(root, 0));
                eprintf("json_unpack_ex: %s [source: %s / line: %d / column: %d]", err.text, err.source, err.line, err.column);
            }
        }
    }
}


#ifndef NDEBUG
void pb_device_dump_infos(const pb_device_t* p_device)
{
    switch ( p_device->type )
    {
        case ICON_BROWSER:
            iprintf("%s - %s", p_device->browser.icon, p_device->browser.iden);
            iprintf("\tactive : %u", p_device->browser.active);
            iprintf("\tcreated : %f", p_device->browser.created);
            iprintf("\tmodified : %f", p_device->browser.modified);
            iprintf("\tnickname : %s", p_device->browser.nickname);
            iprintf("\tmanufacturer : %s", p_device->browser.manufacturer);
            iprintf("\tmodel : %s", p_device->browser.model);
            iprintf("\tapp_version : %hd", p_device->browser.app_version);
            break;

        case ICON_PHONE:
            iprintf("%s - %s", p_device->phone.icon, p_device->phone.iden);
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
