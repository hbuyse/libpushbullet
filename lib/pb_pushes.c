/**
 * @file note.c
 * @author hbuyse
 * @date 12/05/2016
 */

#include <string.h>             // strlen, strdup
#include <jansson.h>
#include <libgen.h>          // basename
#include <magic.h>          // magic_t, magic_open, magic_close, magic_load, magic_file

#include "pb_utils.h"             // iprintf, eprintf, cprintf, gprintf
#include "pb_pushes_priv.h"         // pb_note_t, pb_link_t, pb_file_t, pb_push_t
#include "pb_requests_prot.h"     // pb_requests_post, pb_requests_get, pb_requests_delete, pb_requests_post_multipart
#include "pushbullet.h"

/**
 * @typedef pb_config_t
 * @brief Type definition of the structure pb_config_s
 */
typedef struct pb_config_s pb_config_t;

/**
 * @typedef pb_user_t
 * @brief Type definition of the structure pb_user_s
 */
typedef struct pb_user_s pb_user_t;


/**
 * @brief      Get a JSON note to send with \a pb_requests_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  device_iden  The device identification
 *
 * @return     A string containing the JSON note
 */
static const char* _create_note(const char *title, const char *body, const char *device_iden)
{
    json_t* json = NULL;
    json_error_t err = {0};
    char* res = NULL;

    json = json_pack_ex(&err, 0,
                        "{ssss*ss*ss*}",
                        "type", "note",
                        "title", title,
                        "body", body,
                        "device_iden", device_iden
                        );

    if (json) {
        // json to string
        res = json_dumps(json, 0);

        json_decref(json);
    }

    // Return the JSON as a string
    return (res);
}


/**
 * @brief      Get a JSON link to send with \a pb_requests_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  device_iden  The device identification
 *
 * @return     A string containing the JSON link
 */
static const char* _create_link(const char *title, const char *body, const char *url, const char *device_iden)
{
    json_t* json = NULL;
    json_error_t err = {0};
    const char* res = NULL;


    // If we do not have an URL, it is simply a note, so we send one
    if ( url == NULL )
    {
        res = _create_note(title, body, device_iden);
    }
    else
    {
        // Add type
        json = json_pack_ex(&err, 0,
                            "{ssss*ss*ss*ss}",
                            "type", "link",
                            "title", title,
                            "body", body,
                            "url", url,
                            "device_iden", device_iden
                            );

        if (json) {
            // json to string
            res = json_dumps(json, 0);

            json_decref(json);
        }
    }

    // Return the JSON as a string
    return (res);
}



/**
 * @brief      Get a JSON file to send with \a pb_requests_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  file_name    The file name
 * @param[in]  file_type    The file type
 * @param[in]  file_url     The file url
 * @param[in]  device_iden  The device identification
 *
 * @return     A newly allocated string containing the JSON link. This string has to be freed after.
 */
static const char* _create_file(const char *title, const char *body, const char *file_name, const char *file_type,
                                const char *file_url, const char *device_iden)
{
    json_t* json = NULL;
    json_error_t err = {0};
    const char* res = NULL;


    // If we do not have an URL, it is simply a note, so we send one
    if ( (! file_name) && (! file_type) )
    {
        res = _create_note(title, body, device_iden);
    }
    else
    {
        // Add type
        json = json_pack_ex(&err, 0,
                            "{ssss*ss*ssssss*ss}",
                            "type", "file",
                            "title", title,
                            "body", body,
                            "file_name", file_name,
                            "file_type", file_type,
                            "file_url", file_url,
                            "device_iden", device_iden
                            );

        if (json) {
            // json to string
            res = json_dumps(json, 0);

            json_decref(json);
        }
    }

    // Return the JSON as a string
    return (res);
}



/**
 * \brief      Creates a JSON upload request to send with \a pb_requests_post
 *
 * \param[in]  file_name  The file name
 * \param[in]  file_type  The file type
 *
 * \return     A string containing the JSON upload request
 */
static const char* _pre_upload_request(const char *file_name, const char *file_type
                                       )
{
    json_t* json = NULL;
    json_error_t err = {0};
    const char* res = NULL;


    // If we do not have an URL, it is simply a note, so we send one
    if ( file_name && file_type )
    {
        json = json_pack_ex(&err, 0,
                            "{ssss}",
                            "file_name", file_name
                            );

        if (json) {
            // json to string
            res = json_dumps(json, 0);

            json_decref(json);
        }
    }

    // Return the JSON as a string
    return (res);
}


/**
 * \brief      Update the file url and upload url variables
 *
 * \param      file_url    The file url
 * \param      upload_url  The upload url
 * \param[in]  ur_res      The upload request result
 *
 * \return     { description_of_the_return_value }
 */
static int _post_upload_request(char          **file_url,
                                char          **upload_url,
                                const char    *ur_res,
                                const size_t  ur_res_sz
                                )
{
    int ret                = -1;
    json_t *json = NULL;
    json_error_t err = {0};

    if ((!ur_res) || (ur_res_sz <= 0))
        eprintf("No upload request given");
    else if (!(json = json_loadb(ur_res, ur_res_sz, 0, &err)))
        eprintf("json_loadb: %s [source: %s / line: %d / column: %d]", err.text, err.source, err.line, err.column);
    else if (json_unpack_ex(json, &err, 0, "{ssss}", JSON_KEY_FILE_URL, file_url, JSON_KEY_UPLOAD_URL, upload_url))
        eprintf("json_unpack_ex: %s [source: %s / line: %d / column: %d]", err.text, err.source, err.line, err.column);
    else
        ret = 0;
    return ret;
}

/**
 * \brief      Prepare an upload request structure
 *
 * \param      ur    The upload request structure
 *
 * \return     0 if went well, otherwise there is an error
 */
static int _prepare_upload_request(pb_file_t *file)
{
    int ret = -1;
    magic_t magic_cookie;

    // MAGIC_MIME_TYPE tells magic to return a mime of the file
    if ((magic_cookie = magic_open(MAGIC_MIME_TYPE)) == NULL)
    {
        eprintf("Unable to initialize magic library");
    }
    else
    {
        if ( magic_load(magic_cookie, NULL) != 0 )
        {
            eprintf("Impossible to load magic database - %s", magic_error(magic_cookie));
        }
        else
        {
            file->file_type = strdup(magic_file(magic_cookie, file->file_path));
            ret = 0;
            #ifndef NDEBUG
            gprintf("%s", file->file_type);
            #endif
        }
        magic_close(magic_cookie);
    }

    return ret;
}

/**
 * \brief      Send an upload request for a file
 *
 * \param      result  The result
 * \param[in]  ur      the file informations structure
 * \param[in]  user    The user
 *
 * \return     The HTTP status code to the \a pb_requests_post
 */
static http_code_t _upload_request(char *result, pb_file_t *file, const pb_user_t *user)
{
    const char      *data   = NULL;
    short           res     = 0;
    size_t          result_sz = 0;


    // JSON objects
    data    = _pre_upload_request(file->file_name, file->file_type);

    res     = pb_requests_post(result, &result_sz, API_URL_FILE_REQUEST, pb_user_get_config(user), data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the upload-request (HTTP status code : %d)", res);
        eprintf("%s", result);
    }
    else
    {
#ifndef NDEBUG
        gprintf("\e[1;37m%u\e[0m %s", res, result);
#endif
        _post_upload_request(&file->file_url, &file->upload_url, result, result_sz);
    }

    return (res);
}

/**
 * \brief      Upload the file on the Amazon servers
 *
 * \param      result  The result
 * \param[in]  ur      the file informations structure
 * \param[in]  user    The user
 *
 * \return     { description_of_the_return_value }
 */
static http_code_t _send_request(char            *result,
                                 const pb_file_t *file,
                                 const pb_user_t *user
                                 )
{
    unsigned short     res = 0;
    size_t             result_sz = 0;


    res = pb_requests_post_multipart(result, &result_sz, file->upload_url, pb_user_get_config(user), file);

    if ( res != HTTP_NO_CONTENT )
    {
        eprintf("An error occured when sending the upload-request (HTTP status code : %d)", res);
        eprintf("%s", result);

        return (res);
    }

#ifndef NDEBUG
    else
    {
        gprintf("\e[1;37m%u\e[0m", res);
    }
#endif



    return (res);
}

/**
 * \brief      Free a file structure
 *
 * \param      file  The file structure to free
 */
static void _free_pb_file_t(pb_file_t *file)
{
    pb_free(file->title);
    pb_free(file->body);
    pb_free(file->file_path);
    pb_free(file->file_name);
    pb_free(file->file_url);
    pb_free(file->upload_url);
}


const char* pb_file_get_filepath(const pb_file_t* file)
{
    return (file) ? file->file_path : NULL;
}


http_code_t pb_push_note(char            *result,
                         size_t          *result_sz,
                         const pb_note_t note,
                         const char      *device_nickname,
                         const pb_user_t *user
                         )
{
    const char          *data   = NULL;
    unsigned short      res     = 0;


    // Create the JSON data
    data    = _create_note(note.title, note.body, pb_user_get_device_iden_from_name(user, device_nickname) );

#ifndef NDEBUG
    iprintf("%s", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the note (HTTP status code : %d)", res);
        eprintf("%s", result);
    }

#ifndef NDEBUG
    else
    {
        gprintf("\e[1;37m%u\e[0m %s", res, result);
    }
#endif

    return (res);
}



http_code_t pb_push_link(char            *result,
                         size_t          *result_sz,
                            const pb_link_t link,
                            const char      *device_nickname,
                            const pb_user_t *user
                            )
{
    const char          *data   = NULL;
    unsigned short      res     = 0;


    // Create the JSON data
    data    = _create_link(link.title, link.body, link.url, pb_user_get_device_iden_from_name(user, device_nickname) );

#ifndef NDEBUG
    iprintf("%s", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the note (HTTP status code : %d)", res);
        eprintf("%s", result);
    }
#ifndef NDEBUG
    else
    {
        gprintf("\e[1;37m%u\e[0m %s", res, result);
    }
#endif

    return (res);
}



http_code_t pb_push_file(char            *result,
                         size_t          *result_sz,
                         pb_file_t       *file,
                         const char      *device_nickname,
                         const pb_user_t *user
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

    if ( _send_request(result, file, user) != HTTP_NO_CONTENT )
    {
        return (3);
    }

    data = _create_file(file->title,
                        file->body,
                        file->file_name,
                        file->file_type,
                        file->file_url,
                        pb_user_get_device_iden_from_name(user, device_nickname) );

#ifndef NDEBUG
    iprintf("%s", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the note (HTTP status code : %d)", res);
        eprintf("%s", result);
    }
#ifndef NDEBUG
    else
    {
        gprintf("\e[1;37m%u\e[0m %s", res, result);
    }
#endif

    return (res);
}








