/**
 * @file note.c
 * @author hbuyse
 * @date 12/05/2016
 */

#include <json/json.h>          // json_object, json_object_new_object, json_object_new_string, json_object_object_add,
                                // json_object_to_json_string

#include <pushbullet/urls.h>          // API_URL_PUSHES
#include <pushbullet/user.h>          // pb_user_t
#include <pushbullet/requests.h>          // pb_port
#include <pushbullet/http_code.h>          // HTTP_OK
#include <pushbullet/devices.h>            // pb_get_iden_from_name
#include <pushbullet/logging.h>             // iprintf, eprintf, cprintf, gprintf


/**
 * @brief      Get a JSON note to send with \a pb_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  device_iden  The device identification
 *
 * @return     A string containing the JSON note
 */
static const char* _create_note(const char  *title,
                                const char  *body,
                                const char  *device_iden
                                )
{
    json_object     *root       = NULL;
    json_object     *jstr_type  = NULL;


    // Create the root
    root        = json_object_new_object();


    // Add type
    jstr_type   = json_object_new_string("note");


    json_object_object_add(root, "type", jstr_type);


    // Add title
    if ( title )
    {
        json_object     *jstr_title = json_object_new_string(title);
        json_object_object_add(root, "title", jstr_title);
    }


    // Add body
    if ( body )
    {
        json_object     *jstr_body = json_object_new_string(body);
        json_object_object_add(root, "body", jstr_body);
    }


    // Add device_iden
    if ( device_iden )
    {
        json_object     *jstr_device_iden = json_object_new_string(device_iden);
        json_object_object_add(root, "device_iden", jstr_device_iden);
    }


    // Return the JSON as a string
    return (json_object_to_json_string(root) );
}



/**
 * @brief      Get a JSON link to send with \a pb_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  device_iden  The device identification
 *
 * @return     A string containing the JSON link
 */
static const char* _create_link(const char  *title,
                                const char  *body,
                                const char  *url,
                                const char  *device_iden
                                )
{
    json_object     *root       = NULL;
    json_object     *jstr_type  = NULL;


    // If we do not have an URL, it is simply a note, so we send one
    if ( url == NULL )
    {
        return (_create_note(title, body, device_iden) );
    }


    // Create the root
    root        = json_object_new_object();


    // Add type
    jstr_type   = json_object_new_string("link");


    json_object_object_add(root, "type", jstr_type);


    // Add title
    if ( title )
    {
        json_object     *jstr_title = json_object_new_string(title);
        json_object_object_add(root, "title", jstr_title);
    }


    // Add body
    if ( body )
    {
        json_object     *jstr_body = json_object_new_string(body);
        json_object_object_add(root, "body", jstr_body);
    }


    // Add url
    if ( url )
    {
        json_object     *jstr_url = json_object_new_string(url);
        json_object_object_add(root, "url", jstr_url);
    }


    // Add device_iden
    if ( device_iden )
    {
        json_object     *jstr_device_iden = json_object_new_string(device_iden);
        json_object_object_add(root, "device_iden", jstr_device_iden);
    }


    // Return the JSON as a string
    return (json_object_to_json_string(root) );
}



unsigned short pb_push_note(char            *result,
                            const pb_note_t note,
                            const char      *device_nickname,
                            const pb_user_t user
                            )
{
    const char          *data   = NULL;
    unsigned short      res     = 0;


    // Create the JSON data
    data    = _create_note(note.title, note.body, pb_get_iden_from_name(user, device_nickname) );

    #ifdef __DEBUG__
    iprintf("%s\n", data);
    #endif


    // Send the datas
    res     = pb_post(result, API_URL_PUSHES, user, (char *) data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the note (HTTP status code : %d)\n", res);
        eprintf("%s\n", result);
    }

    #ifdef __DEBUG__
    else
    {
        iprintf("%s\n", result);
    }
    #endif

    return (res);
}



unsigned short pb_push_link(char            *result,
                            const pb_link_t link,
                            const char      *device_nickname,
                            const pb_user_t user
                            )
{
    const char          *data   = NULL;
    unsigned short      res     = 0;


    // Create the JSON data
    data    = _create_link(link.title, link.body, link.url, pb_get_iden_from_name(user, device_nickname) );

    #ifdef __DEBUG__
    iprintf("%s\n", data);
    #endif


    // Send the datas
    res     = pb_post(result, API_URL_PUSHES, user, (char *) data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the note (HTTP status code : %d)\n", res);
        eprintf("%s\n", result);
    }
    else
    {
        #ifdef __DEBUG__
        iprintf("%s\n", result);
        #endif
    }

    return (res);
}