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
#include "pb_internal.h"
#include "pushbullet.h"


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
 * @brief Maximum length of a MIME type
 * @details According to RFC 4288 "Media Type Specifications and Registration Procedures", type (eg. "application") and
 * subtype (eg "vnd...") both can
 *          be max 127 characters. (256 - 0x100)
 */
#define     MIME_TYPE_MAX_LENGTH 0x100


/**
 * @typedef pb_note_t
 * @brief Type definition of the structure pb_note_s
 */
typedef struct pb_note_s pb_note_t;


/**
 * @typedef pb_link_t
 * @brief Type definition of the structure pb_link_s
 */
typedef struct pb_link_s pb_link_t;


/**
 * @typedef pb_file_t
 * @brief Type definition of the structure pb_file_s
 */
typedef struct pb_file_s pb_file_t;


/**
 * @typedef pb_push_t
 * @brief Type definition of the structure pb_push_s
 */
typedef struct pb_push_s pb_push_t;

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
 * @struct pb_note_s
 * @brief Structure containing all the informations concerning a PushBullet note
 */
struct pb_note_s {
    char *title;          ///< Push's title
    char *body;          ///< Push's body
};


/**
 * @struct pb_link_s
 * @brief Structure containing all the informations concerning a PushBullet link
 */
struct pb_link_s {
    char *title;          ///< Push's title
    char *body;          ///< Push's body
    char *url;          ///< Push's url
};


/**
 * @struct pb_file_s
 * @brief Structure containing all the informations concerning a PushBullet Upload request for a file
 */
struct pb_file_s {
    char *title;          ///< Push's title
    char *body;          ///< Push's body
    char *file_path;          ///< File path
    char *file_name;          ///< File name
    char *file_type;          ///< File's MIME type
    char *file_url;          ///< File url
    char *upload_url;          ///< File upload url
};


/**
 * @struct pb_push_s
 * @brief Structure containing all the informations concerning a PushBullet push
 */
struct pb_push_s {
    unsigned char active;          ///< Push's activity
    const char *body;          ///< Push's body
    double created;          ///< Push's creation
    const char *direction;          ///< Push's direction
    unsigned char dismissed;          ///< Is the push dismissed?
    const char *iden;          ///< Push's identification
    double modified;          ///< Push's last modification
    const char *receiver_email;          ///< Receiver email of the push
    const char *receiver_email_normalized;          ///< Receiver email normalized of the push
    const char *receiver_iden;          ///< Receiver identification of the push
    const char *sender_email;          ///< Sender's email of the push
    const char *sender_email_normalized;          ///< Sender's email normalized of the push
    const char *sender_iden;          ///< Sender's identification of the push
    const char *sender_name;          ///< Sender's name of the push
    const char *title;          ///< Push's title
    const char *type;          ///< Push's type
};


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
    data    = _create_note(note.title, note.body, pb_devices_get_iden_from_name(user, device_nickname) );

#ifdef __DEBUG__
    iprintf("%s\n", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

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
    data    = _create_link(link.title, link.body, link.url, pb_devices_get_iden_from_name(user, device_nickname) );

#ifdef __DEBUG__
    iprintf("%s\n", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

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
                        pb_devices_get_iden_from_name(user, device_nickname) );

#ifdef __DEBUG__
    iprintf("%s\n", data);
#endif


    // Send the datas
    res     = pb_requests_post(result, result_sz, API_URL_PUSHES, pb_user_get_config(user), (char *) data);

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
        eprintf("%s\n", err->message);
    }
    else
    {
        root = json_parser_get_root(parser);

        if ( ! JSON_NODE_HOLDS_OBJECT(root) )
        {
            eprintf("Not a JsonObject\n");
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
        eprintf("Unable to initialize magic library\n");
    }
    else
    {
        if ( magic_load(magic_cookie, NULL) != 0 )
        {
            eprintf("Impossible to load magic database - %s\n", magic_error(magic_cookie));
        }
        else
        {
            file->file_type = strdup(magic_file(magic_cookie, file->file_path));
            ret = 0;
            #ifdef __DEBUG__
            gprintf("%s\n", file->file_type);
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