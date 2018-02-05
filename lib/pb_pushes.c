/**
 * @file note.c
 * @author hbuyse
 * @date 12/05/2016
 */

#include <string.h>             // strlen, strdup
#include <json-glib/json-glib.h>          // JsonObject, json_object_new, json_object_new_string, json_object_object_add,
                                // json_object_to_json_string
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
static const char* _create_note(const char *title, const char *body, const char *device_iden);



/**
 * @brief      Get a JSON link to send with \a pb_requests_post
 *
 * @param[in]  title        The title
 * @param[in]  body         The body
 * @param[in]  device_iden  The device identification
 *
 * @return     A string containing the JSON link
 */
static const char* _create_link(const char *title, const char *body, const char *url, const char *device_iden);



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
static const char* _create_file(const char  *title,
                                const char  *body,
                                const char  *file_name,
                                const char  *file_type,
                                const char  *file_url,
                                const char  *device_iden);



/**
 * \brief      Creates a JSON upload request to send with \a pb_requests_post
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
static int _post_upload_request(char **file_url, char **upload_url, const char *ur_res);


/**
 * \brief      Prepare an upload request structure
 *
 * \param      ur    The upload request structure
 *
 * \return     0 if went well, otherwise there is an error
 */
static int _prepare_upload_request(pb_file_t *ur);


/**
 * \brief      Send an upload request for a file
 *
 * \param      result  The result
 * \param[in]  ur      the file informations structure
 * \param[in]  user    The user
 *
 * \return     The HTTP status code to the \a pb_requests_post
 */
static http_code_t _upload_request(char *result, pb_file_t *ur, const pb_user_t *user);


/**
 * \brief      Upload the file on the Amazon servers
 *
 * \param      result  The result
 * \param[in]  ur      the file informations structure
 * \param[in]  user    The user
 *
 * \return     { description_of_the_return_value }
 */
static http_code_t _send_request(char *result, const pb_file_t *ur, const pb_user_t *user);


/**
 * \brief      Free a file structure
 *
 * \param      file  The file structure to free
 */
static void _free_pb_file_t(pb_file_t *file);



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

#ifdef __TRACES__
    iprintf("%s", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the note (HTTP status code : %d)", res);
        eprintf("%s", result);
    }

#ifdef __TRACES__
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

#ifdef __TRACES__
    iprintf("%s", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the note (HTTP status code : %d)", res);
        eprintf("%s", result);
    }
#ifdef __TRACES__
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

#ifdef __TRACES__
    iprintf("%s", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

    if ( res != HTTP_OK )
    {
        eprintf("An error occured when sending the note (HTTP status code : %d)", res);
        eprintf("%s", result);
    }
#ifdef __TRACES__
    else
    {
        gprintf("\e[1;37m%u\e[0m %s", res, result);
    }
#endif

    return (res);
}



static const char* _create_note(const char  *title,
                                const char  *body,
                                const char  *device_iden
                                )
{
    JsonGenerator* parser = json_generator_new();
    JsonNode       *root = NULL;
    JsonObject     *obj       = json_object_new();
    char* res = NULL;


    // Add type
    json_object_set_string_member(obj, "type", "note");

    // Add title
    if ( title )
    {
        json_object_set_string_member(obj, "title", title);
    }

    // Add body
    if ( body )
    {
        json_object_set_string_member(obj, "body", body);
    }

    // Add device_iden
    if ( device_iden )
    {
        json_object_set_string_member(obj, "device_iden", device_iden);
    }

    root = json_node_init_object(json_node_alloc (), obj);

    // The node is copied by the generator object, so it can be safely freed after calling this function.
    json_generator_set_root(parser, root);

    // So, free the node
    json_node_free(root);

    // Returns : a newly allocated buffer holding a JSON data stream. Use g_free() to free the allocated resources.
    res = json_generator_to_data(parser, NULL);

    g_free(parser);

    // Return the JSON as a string
    return (res);
}



static const char* _create_link(const char  *title,
                                const char  *body,
                                const char  *url,
                                const char  *device_iden
                                )
{
    JsonGenerator* parser = json_generator_new();
    JsonNode       *root = NULL;
    JsonObject     *obj       = json_object_new();
    const char* res = NULL;


    // If we do not have an URL, it is simply a note, so we send one
    if ( url == NULL )
    {
        res = _create_note(title, body, device_iden);
    }
    else
    {
        // Add type
        json_object_set_string_member(obj, "type", "link");

        // Add title
        if ( title )
        {
            json_object_set_string_member(obj, "title", title);
        }

        // Add body
        if ( body )
        {
            json_object_set_string_member(obj, "body", body);
        }

        // Add url
        if ( url )
        {
            json_object_set_string_member(obj, "url", url);
        }

        // Add device_iden
        if ( device_iden )
        {
            json_object_set_string_member(obj, "device_iden", device_iden);
        }

        root = json_node_init_object(json_node_alloc (), obj);

        // The node is copied by the generator object, so it can be safely freed after calling this function.
        json_generator_set_root(parser, root);

        // So, free the node
        json_node_free(root);

        // Returns : a newly allocated buffer holding a JSON data stream. Use g_free() to free the allocated resources.
        res = json_generator_to_data(parser, NULL);

        g_free(parser);
    }

    // Return the JSON as a string
    return (res);
}



static const char* _create_file(const char  *title,
                                const char  *body,
                                const char  *file_name,
                                const char  *file_type,
                                const char  *file_url,
                                const char  *device_iden
                                )
{
    JsonGenerator* parser = json_generator_new();
    JsonNode       *root = NULL;
    JsonObject     *obj       = json_object_new();
    const char* res = NULL;


    // If we do not have an URL, it is simply a note, so we send one
    if ( (! file_name) && (! file_type) )
    {
        res =_create_note(title, body, device_iden);
    }
    else
    {
        // Add type
        json_object_set_string_member(obj, "type", "link");

        // Add title
        if ( title )
        {
            json_object_set_string_member(obj, "title", title);
        }

        // Add body
        if ( body )
        {
            json_object_set_string_member(obj, "body", body);
        }

        // Add file_name
        if ( file_name )
        {
            json_object_set_string_member(obj, "file_name", file_name);
        }

        // Add file_type
        if ( file_type )
        {
            json_object_set_string_member(obj, "file_type", file_type);
        }

        // Add file_url
        if ( file_url )
        {
            json_object_set_string_member(obj, "file_url", file_url);
        }

        // Add device_iden
        if ( device_iden )
        {
            json_object_set_string_member(obj, "device_iden", device_iden);
        }

        root = json_node_init_object(json_node_alloc (), obj);

        // The node is copied by the generator object, so it can be safely freed after calling this function.
        json_generator_set_root(parser, root);

        // So, free the node
        json_node_free(root);

        // Returns : a newly allocated buffer holding a JSON data stream. Use g_free() to free the allocated resources.
        res = json_generator_to_data(parser, NULL);

        g_free(parser);
    }

    // Return the JSON as a string
    return res;
}



static const char* _pre_upload_request(const char   *file_name,
                                       const char   *file_type
                                       )
{
    JsonGenerator* parser = json_generator_new();
    JsonNode       *root = NULL;
    JsonObject     *obj       = json_object_new();
    char* res = NULL;

    // Add title
    if ( file_name && file_type )
    {
        json_object_set_string_member(obj, "file_name", file_name);
        json_object_set_string_member(obj, "file_type", file_type);

        root = json_node_init_object(json_node_alloc (), obj);

        // The node is copied by the generator object, so it can be safely freed after calling this function.
        json_generator_set_root(parser, root);

        // So, free the node
        json_node_free(root);

        // Returns : a newly allocated buffer holding a JSON data stream. Use g_free() to free the allocated resources.
        res = json_generator_to_data(parser, NULL);

        g_free(parser);
    }

    // Return the JSON as a string
    return (res);
}



static int _post_upload_request(char          **file_url,
                                char          **upload_url,
                                const char    *ur_res
                                )
{
    int ret                = -1;
    GError *err            = NULL;
    JsonParser     *parser = json_parser_new();
    JsonNode     *root     = NULL;
    JsonObject     *obj    = NULL;

    if ( ! json_parser_load_from_data(parser, ur_res, strlen(ur_res), &err) )
    {
        eprintf("%s", err->message);
    }
    else
    {
        root = json_parser_get_root(parser);

        if ( ! JSON_NODE_HOLDS_OBJECT(root) )
        {
            eprintf("Not a JsonObject");
        }
        else
        {
            obj = json_node_get_object(root);

            *file_url = strdup(json_object_get_string_member(obj, JSON_KEY_FILE_URL));
            *upload_url = strdup(json_object_get_string_member(obj, JSON_KEY_UPLOAD_URL));

            ret = 0;
        }
    }
    return ret;
}

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
            #ifdef __TRACES__
            gprintf("%s", file->file_type);
            #endif
        }
        magic_close(magic_cookie);
    }

    return ret;
}



static http_code_t _upload_request(char              *result,
                                   pb_file_t         *file,
                                   const pb_user_t   *user
                                   )
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
#ifdef __TRACES__
        gprintf("\e[1;37m%u\e[0m %s", res, result);
#endif
        _post_upload_request(&file->file_url, &file->upload_url, result);
    }

    return (res);
}



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

#ifdef __TRACES__
    else
    {
        gprintf("\e[1;37m%u\e[0m", res);
    }
#endif



    return (res);
}



static void _free_pb_file_t(pb_file_t *file)
{
    pb_free(file->title);
    pb_free(file->body);
    pb_free(file->file_path);
    pb_free(file->file_name);
    pb_free(file->file_url);
    pb_free(file->upload_url);
}