/**
 * @file note.c
 * @author hbuyse
 * @date 12/05/2016
 */

#include <string.h>             // strlen, strdup
#include <json/json.h>          // json_object, json_object_new_object, json_object_new_string, json_object_object_add,
                                // json_object_to_json_string
#include <libgen.h>          // basename

#include <pushbullet/structures.h>          // pb_device_t, pb_user_t, pb_browser_t, pb_phone_t, pb_file_t
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
 * \brief    JSON key to get the future url of the file
 * @details  Field containing the URL where the file will be available after it is uploaded.
 */
#define     JSON_KEY_FILE_URL           "file_url"


/**
 * \brief    JSON key to get the upload url of the file
 * @details  Field containing the URL to POST the file to.
 *           The file must be posted using multipart/form-data encoding.
 */
#define     JSON_KEY_UPLOAD_URL         "upload_url"


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
 * @brief      Get a JSON file to send with \a pb_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  file_name    The file name
 * @param[in]  file_type    The file type
 * @param[in]  file_url     The file url
 * @param[in]  device_iden  The device identification
 *
 * @return     A string containing the JSON link
 */
static const char* _create_file(const char  *title,
                                const char  *body,
                                const char  *file_name,
                                const char  *file_type,
                                const char  *file_url,
                                const char  *device_iden);



/**
 * \brief      Creates a JSON upload request to send with \a pb_post
 *
 * \param[in]  file_name  The file name
 * \param[in]  file_type  The file type
 *
 * \return     A string containing the JSON upload request
 */
static const char* _pre_upload_request(const char *file_name, const char *file_type);


/**
 * \brief      Update the file url and upload url variables
 *
 * \param      file_url    The file url
 * \param      upload_url  The upload url
 * \param[in]  ur_res      The upload request result
 *
 * \return     { description_of_the_return_value }
 */
static unsigned char _post_upload_request(char **file_url, char **upload_url, const char *ur_res);


/**
 * \brief      Prepare an upload request structure
 *
 * \param      ur    The upload request structure
 *
 * \return     0 if went well, otherwise there is an error
 */
static unsigned short _prepare_upload_request(pb_file_t *ur);


/**
 * \brief      Send an upload request for a file
 *
 * \param      result  The result
 * \param[in]  ur      the file informations structure
 * \param[in]  user    The user
 *
 * \return     The HTTP status code to the \a pb_post
 */
static unsigned short _upload_request(char *result, pb_file_t *ur, const pb_user_t user);


/**
 * \brief      Upload the file on the Amazon servers
 *
 * \param      result  The result
 * \param[in]  ur      the file informations structure
 * \param[in]  user    The user
 *
 * \return     { description_of_the_return_value }
 */
static unsigned short _send_request(char *result, const pb_file_t ur, const pb_user_t user);


/**
 * \brief      Free a file structure
 *
 * \param      file  The file structure to free
 */
static void _free_pb_file_t(pb_file_t *file);


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
                            const pb_user_t user,
                            const char      *device_nickname
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



unsigned short pb_push_file(char            *result,
                            pb_file_t       *file,
                            const char      *device_nickname,
                            const pb_user_t user
                            )
{
    const char     *data = NULL;
    unsigned short      res     = 0;


    if ( _prepare_upload_request(file) != 0 )
    {
        return (1);
    }

    if ( _upload_request(result, file, user) != HTTP_OK )
    {
        return (3);
    }

    if ( _send_request(result, *file, user) != HTTP_NO_CONTENT )
    {
        return (3);
    }

    data = _create_file(file->title,
                        file->body,
                        file->file_name,
                        file->file_type,
                        file->file_url,
                        pb_get_iden_from_name(user, device_nickname) );

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



static const char* _create_file(const char  *title,
                                const char  *body,
                                const char  *file_name,
                                const char  *file_type,
                                const char  *file_url,
                                const char  *device_iden
                                )
{
    json_object     *root       = NULL;
    json_object     *jstr_type  = NULL;


    // If we do not have an URL, it is simply a note, so we send one
    if ( (! file_name) && (! file_type) )
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

    // Add file_name
    if ( file_name )
    {
        json_object     *jstr_file_name = json_object_new_string(file_name);
        json_object_object_add(root, "file_name", jstr_file_name);
    }

    // Add file_type
    if ( file_type )
    {
        json_object     *jstr_file_type = json_object_new_string(file_type);
        json_object_object_add(root, "file_type", jstr_file_type);
    }

    // Add file_url
    if ( file_url )
    {
        json_object     *jstr_file_url = json_object_new_string(file_url);
        json_object_object_add(root, "file_url", jstr_file_url);
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



static const char* _pre_upload_request(const char   *file_name,
                                       const char   *file_type
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



static unsigned char _post_upload_request(char          **file_url,
                                          char          **upload_url,
                                          const char    *ur_res
                                          )
{
    json_object     *root           = NULL;
    json_object     *jfile_url      = NULL;
    json_object     *jupload_url    = NULL;


    if ( ! ur_res )
    {
        return (2);
    }

    root = json_tokener_parse(ur_res);
    json_object_object_get_ex(root, JSON_KEY_FILE_URL, &jfile_url);
    json_object_object_get_ex(root, JSON_KEY_UPLOAD_URL, &jupload_url);

    if ( jfile_url && jupload_url )
    {
        *file_url   = (char *) calloc(strlen(json_object_get_string(jfile_url) + 1), sizeof(char) );
        *upload_url = (char *) calloc(strlen(json_object_get_string(jupload_url) + 1), sizeof(char) );

        strncpy(*file_url, json_object_get_string(jfile_url), strlen(json_object_get_string(jfile_url) ) );
        strncpy(*upload_url, json_object_get_string(jupload_url), strlen(json_object_get_string(jupload_url) ) );

        return (0);
    }
    else
    {
        *file_url   = NULL;
        *upload_url = NULL;

        return (1);
    }
}



static unsigned short _prepare_upload_request(pb_file_t *file)
{
    char        buff[BUF_MAX_LENGTH];     // Buffer for the result
    FILE        *fp = NULL;     // Pointer to the file
    char        cmd[MIME_TYPE_CMD_LENGTH(file->file_path) + 1];      // Buffer for the system command


    // Get the name of the file
    // Why strdup? Because basename() may modify the contents of path, so it may be desirable to pass a copy when
    // calling one of these functions.
    file->file_name = basename(strdup(file->file_path) );


    // Get the MIME type of the file using the system command `file`
    snprintf(cmd, MIME_TYPE_CMD_LENGTH(file->file_path) + 1, MIME_TYPE_CMD_FORMAT, file->file_path);

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
        char     *ptr       = NULL;
        unsigned char     i = 0;

        for ( i = 0; i < MIME_TYPE_MAX_ITER; ++i )
        {
            ptr = strtok( (i == 0) ? buff : NULL, ":;");
        }

        snprintf(file->file_type, strlen(ptr) + 1, "%s", ptr);
    }
    else
    {
        snprintf(file->file_type, MIME_TYPE_DEFAULT_LENGTH + 1, "%s", MIME_TYPE_DEFAULT);
    }

    return (0);
}



static unsigned short _upload_request(char              *result,
                                      pb_file_t         *file,
                                      const pb_user_t   user
                                      )
{
    const char      *data   = NULL;
    short           res     = 0;


    // JSON objects
    data    = _pre_upload_request(file->file_name, file->file_type);

    res     = pb_post(result, API_URL_FILE_REQUEST, user, data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the upload-request (HTTP status code : %d)\n", res);
        eprintf("%s\n", result);
    }
    else
    {
#ifdef __DEBUG__
        gprintf("\e[1;37m%u\e[0m %s\n", res, result);
#endif
        _post_upload_request(&file->file_url, &file->upload_url, result);
    }

    return (res);
}



static unsigned short _send_request(char            *result,
                                    const pb_file_t file,
                                    const pb_user_t user
                                    )
{
    unsigned short     res = 0;


    res = pb_post_multipart(result, file.upload_url, user, file);

    if ( res != HTTP_NO_CONTENT )
    {
        eprintf("An error occured when sending the upload-request (HTTP status code : %d)\n", res);
        eprintf("%s\n", result);

        return (res);
    }

#ifdef __DEBUG__
    else
    {
        gprintf("\e[1;37m%u\e[0m\n", res);
    }
#endif



    return (res);
}



static void _free_pb_file_t(pb_file_t *file)
{
    if ( file->title )
    {
        free(file->title);
    }

    if ( file->body )
    {
        free(file->body);
    }

    if ( file->file_path )
    {
        free(file->file_path);
    }

    if ( file->file_name )
    {
        free(file->file_name);
    }

    if ( file->file_url )
    {
        free(file->file_url);
    }

    if ( file->upload_url )
    {
        free(file->upload_url);
    }
}