/**
 * @file note.c
 * @author hbuyse
 * @date 12/05/2016
 */

#include <string.h>             // strlen, strdup
#include <json/json.h>          // json_object, json_object_new_object, json_object_new_string, json_object_object_add,
                                // json_object_to_json_string
#include <libgen.h>          // basename

#include <pushbullet/structures.h>          // pb_device_t, pb_user_t, pb_browser_t, pb_phone_t, pb_upload_request_t
#include <pushbullet/urls.h>          // API_URL_PUSHES
#include <pushbullet/user.h>          // pb_user_t
#include <pushbullet/requests.h>          // pb_port
#include <pushbullet/http_code.h>          // HTTP_OK
#include <pushbullet/devices.h>            // pb_get_iden_from_name
#include <pushbullet/logging.h>             // iprintf, eprintf, cprintf, gprintf


/**
 * @brief Maximum length of the buffer (4ko - 4096 - 0x1000)
 */
#define     BUF_MAX_LENGTH              0x1000


/**
 * @brief Command format to get the MIME type of a file
 */
#define MIME_TYPE_CMD_FORMAT            "file -i %s"


/**
 * \brief      Macro to get the MIME_TYPE_CMD_FORMAT length
 *
 * \param      file_path  The file path
 *
 * \return     MIME_TYPE_CMD_FORMAT length
 */
#define MIME_TYPE_CMD_LENGTH(file_path) strlen(file_path) + 8


/**
 * @brief Maximum iteration to get the file's MIME type using the system command "file"
 */
#define     MIME_TYPE_MAX_ITER          2


/**
 * @brief Default MIME type
 */
#define     MIME_TYPE_DEFAULT           "text/plain"


/**
 * @brief Default MIME type length
 */
#define     MIME_TYPE_DEFAULT_LENGTH    10


/**
 * @brief      Get a JSON note to send with \a pb_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  device_iden  The device identification
 *
 * @return     A string containing the JSON note
 */
static const char* _create_note(const char *title, const char *body, const char *device_iden);



/**
 * @brief      Get a JSON link to send with \a pb_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  device_iden  The device identification
 *
 * @return     A string containing the JSON link
 */
static const char* _create_link(const char *title, const char *body, const char *url, const char *device_iden);



/**
 * \brief      Creates a JSON upload request to send with \a pb_post
 *
 * \param[in]  file_name  The file name
 * \param[in]  file_type  The file type
 *
 * \return     A string containing the JSON upload request
 */
static const char* _create_upload_request(const char *file_name, const char *file_type);



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
        gprintf("\e[1;37m%u\e[0m %s\n", res, result);
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
        gprintf("\e[1;37m%u\e[0m %s\n", res, result);
#endif
    }

    return (res);
}



unsigned short pb_prepare_upload_request(pb_upload_request_t *ur)
{
    char        buff[BUF_MAX_LENGTH];     // Buffer for the result
    FILE        *fp = NULL;     // Pointer to the file
    char        cmd[MIME_TYPE_CMD_LENGTH(ur->file_path) + 1];      // Buffer for the system command


    // Get the name of the file
    // Why strdup? Because basename() may modify the contents of path, so it may be desirable to pass a copy when
    // calling one of these functions.
    ur->file_name = basename(strdup(ur->file_path) );


    // Get the MIME type of the file using the system command `file`
    snprintf(cmd, MIME_TYPE_CMD_LENGTH(ur->file_path) + 1, MIME_TYPE_CMD_FORMAT, ur->file_path);

    fp = popen(cmd, "r");

    if ( ! fp )
    {
#ifdef __DEBUG__
        eprintf("Failed to run command \"%s\".\n", cmd);
#endif

        return (1);
    }


    // Make a system call
    if ( fgets(buff, sizeof(buff), fp) )
    {
        char                *ptr    = NULL;
        unsigned char       i       = 0;

        for ( i = 0; i < MIME_TYPE_MAX_ITER; ++i )
        {
            ptr = strtok( (i == 0) ? buff : NULL, ":;");
        }

        snprintf(ur->file_type, strlen(ptr) + 1, "%s", ptr);
    }
    else
    {
        snprintf(ur->file_type, MIME_TYPE_DEFAULT_LENGTH + 1, "%s", MIME_TYPE_DEFAULT);
    }

    return (0);
}



unsigned short pb_upload_request(char                       *result,
                                 const pb_upload_request_t  ur,
                                 const pb_user_t            user
                                 )
{
    const char      *data   = NULL;
    short           res     = 0;


    // JSON objects
    data    = _create_upload_request(ur.file_name, ur.file_type);

    res     = pb_post(result, API_URL_FILE_REQUEST, user, data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the upload-request (HTTP status code : %d)\n", res);
        eprintf("%s\n", result);
    }

#ifdef __DEBUG__
    else
    {
        gprintf("\e[1;37m%u\e[0m %s\n", res, result);
    }
#endif

    return (res);
}



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



static const char* _create_upload_request(const char    *file_name,
                                          const char    *file_type
                                          )
{
    json_object     *root = NULL;


    // Create the root
    root = json_object_new_object();


    // Add title
    if ( file_name && file_type )
    {
        json_object     *jstr_file_name = json_object_new_string(file_name);
        json_object     *jstr_file_type = json_object_new_string(file_type);
        json_object_object_add(root, "file_name", jstr_file_name);
        json_object_object_add(root, "file_type", jstr_file_type);
    }


    // Return the JSON as a string
    return (json_object_to_json_string(root) );
}