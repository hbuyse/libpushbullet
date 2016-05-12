/**
 * @file note.c
 * @author hbuyse
 * @date 12/05/2016
 */

#include <stdio.h>          // fprintf, stderr
#include <json/json.h>          // json_object, json_object_new_object, json_object_new_string, json_object_object_add,
                                // json_object_to_json_string

#include <urls.h>          // API_URL_PUSHES
#include <user.h>          // PB_user_t
#include <requests.h>          // pb_port
#include <http_code.h>          // HTTP_OK


static const char* _create_note(const char  *title,
                                const char  *body,
                                const char  *device_iden
                                )
{
    json_object     *root               = json_object_new_object();


    // Add type
    json_object     *jstr_type          = json_object_new_string("note");


    json_object_object_add(root, "type", jstr_type);


    // Add title
    json_object     *jstr_title         = json_object_new_string(title);
    json_object_object_add(root, "title", jstr_title);


    // Add body
    json_object     *jstr_body          = json_object_new_string(body);
    json_object_object_add(root, "body", jstr_body);


    // Add device_iden
    json_object     *jstr_device_iden   = json_object_new_string(device_iden);
    json_object_object_add(root, "device_iden", jstr_device_iden);


    // Return the JSON as a string
    return (json_object_to_json_string(root) );
}



static const char* _create_link(const char  *title,
                                const char  *body,
                                const char  *url,
                                const char  *device_iden
                                )
{
    json_object     *root               = json_object_new_object();


    // Add type
    json_object     *jstr_type          = json_object_new_string("link");


    json_object_object_add(root, "type", jstr_type);


    // Add title
    json_object     *jstr_title         = json_object_new_string(title);
    json_object_object_add(root, "title", jstr_title);


    // Add body
    json_object     *jstr_body          = json_object_new_string(body);
    json_object_object_add(root, "body", jstr_body);


    // Add url
    json_object     *jstr_url           = json_object_new_string(url);
    json_object_object_add(root, "url", jstr_url);


    // Add device_iden
    json_object     *jstr_device_iden   = json_object_new_string(device_iden);
    json_object_object_add(root, "device_iden", jstr_device_iden);


    // Return the JSON as a string
    return (json_object_to_json_string(root) );
}



unsigned short pb_push_note(char            *result,
                            const char      *title,
                            const char      *body,
                            const char      *device_iden,
                            const PB_user_t user
                            )
{
    const char          *data   = NULL;
    unsigned short      res     = 0;


    // Create the JSON data
    data    = _create_note(title, body, device_iden);

    #ifdef __DEBUG__
    fprintf(stdout, "\e[1m[%s]\e[0m %s\n", __func__, data);
    #endif


    // Send the datas
    res     = pb_post(result, API_URL_PUSHES, user.token_key, (char *) data);

    if ( res != HTTP_OK )
    {
        fprintf(stderr,
                "\e[1;31m[%s]\e[0m An error occured when sending the note (HTTP status code : %d)\n",
                __func__,
                res);
        fprintf(stderr, "\e[1;31m[%s]\e[0m %s\n", __func__, result);
    }

    #ifdef __DEBUG__
    else
    {
        fprintf(stdout, "\e[1m[%s]\e[0m %s\n", __func__, result);
    }
    #endif

    return (res);
}



unsigned short pb_push_link(char            *result,
                            const char      *title,
                            const char      *body,
                            const char      *url,
                            const char      *device_iden,
                            const PB_user_t user
                            )
{
    const char          *data   = NULL;
    unsigned short      res     = 0;


    // Create the JSON data
    data    = _create_link(title, body, url, device_iden);

    #ifdef __DEBUG__
    fprintf(stdout, "\e[1m[%s]\e[0m %s\n", __func__, data);
    #endif


    // Send the datas
    res     = pb_post(result, API_URL_PUSHES, user.token_key, (char *) data);

    if ( res != HTTP_OK )
    {
        fprintf(stderr,
                "\e[1;31m[%s]\e[0m An error occured when sending the note (HTTP status code : %d)\n",
                __func__,
                res);
        fprintf(stderr, "\e[1;31m[%s]\e[0m %s\n", __func__, result);
    }
    else
    {
        #ifdef __DEBUG__
        fprintf(stdout, "\e[1m[%s]\e[0m %s\n", __func__, result);
        #endif
    }

    return (res);
}