/**
 * @file requests.c
 * @author hbuyse
 * @date 08/05/2016
 */

#include <stdlib.h>          // realloc, free
#include <string.h>          // memcpy
#include <curl/curl.h>          // CURL, CURLcode, struct curl_slist, curl_slist_append, curl_easy_init,
                                // curl_easy_setopt, curl_easy_perform, curl_easy_cleanup, curl_slist_free_all

#include "pb_utils.h"             // iprintf, eprintf, cprintf, gprintf
#include "pb_requests_priv.h"             // pb_file_get_filepath
#include "pb_pushes_prot.h"             // pb_file_get_filepath
#include "pushbullet.h"          // NUMBER_PROXIES, PROXY_MAX_LENGTH, HTTPS_PROXY


/**
 * @brief Write a downloaded element in the memory
 *
 * @param contents Downloaded content
 * @param size Size of the buffer
 * @param nmemb Size of each element of that buffer
 * @param userp The pointer to the memory
 *
 * @return Return the size of the downloaded element
 */
static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp);


http_code_t pb_requests_get(char              **result,
                            size_t            *length,
                            const char        *url_request,
                            const pb_config_t *p_config
                            )
{
    http_code_t http_code = HTTP_UNKNOWN_CODE;
    struct memory_struct_s ms = { .data = 0, .size = 0};


    /*  Start a libcurl easy session
     */
    CURL     *s = curl_easy_init();


    if ( ! s )
    {
        eprintf("curl_easy_init() could not be initiated.\n");
    }
    else
    {
        CURLcode     r = CURLE_OK;
        struct curl_slist     *http_headers = NULL;

        http_headers = curl_slist_append(http_headers, CONTENT_TYPE_JSON);


        /*  Specify URL to get
         *  Specify the user using the token key
         *  Specify the proxy
         *  Specify the timeout
         *  Specify the data we are about to send
         *  Specify the HTTP header
         *  Send incomming data to the write_memory_callback method
         */
        curl_easy_setopt(s, CURLOPT_USERAGENT, CURL_USERAGENT);
        curl_easy_setopt(s, CURLOPT_HTTPHEADER, http_headers);
        curl_easy_setopt(s, CURLOPT_URL, url_request);
        curl_easy_setopt(s, CURLOPT_USERPWD, pb_config_get_token_key(p_config));
        curl_easy_setopt(s, CURLOPT_PROXY, pb_config_get_proxy(p_config) );
        curl_easy_setopt(s, CURLOPT_TIMEOUT, pb_config_get_timeout(p_config) );
        curl_easy_setopt(s, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(s, CURLOPT_WRITEDATA, (void*) &ms);

        /* Get data && http status code
         */
        r = curl_easy_perform(s);

        /* Checking errors
         */
        if ( r != CURLE_OK )
        {
            eprintf("curl_easy_perform() failed: %s", curl_easy_strerror(r) );
        }
        else
        {
            if (result && ms.data)
            {
                *result = (char*) calloc(ms.size + 1, sizeof(char));
                memcpy(*result, ms.data, ms.size);
            }

            if (length)
            {
                *length = ms.size;
            }
            
            pb_free(ms.data);
        }

        curl_easy_getinfo(s, CURLINFO_RESPONSE_CODE, &http_code);

        #ifdef __TRACES__
        if (length && result)
        {
            if ( http_code == HTTP_OK )
            {
                gprintf("\e[37m%s %u\e[0m %zu %s", url_request, http_code, *length, *result);
            }
            else
            {
                eprintf("\e[37m%s %u\e[0m %zu %s", url_request, http_code, *length, *result);
            }
        }
        #endif

        curl_easy_cleanup(s);
        curl_slist_free_all(http_headers);
    }

    return (http_code);
}



http_code_t pb_requests_post(char              *result,
                       size_t            *length, 
                       const char        *url_request,
                       const pb_config_t *p_config,
                       const char        *data
                       )
{
    /*  Documentation on CURL for C can be found at http://curl.haxx.se/libcurl/c/
     */
    unsigned short http_code  = HTTP_UNKNOWN_CODE;
    struct memory_struct_s ms = {0};


    /*  Start a libcurl easy session
     */
    CURL     *s = curl_easy_init();


    if ( ! s )
    {
        eprintf("curl_easy_init() could not be initiated.\n");
    }
    else
    {
        CURLcode     r = CURLE_OK;
        struct curl_slist     *http_headers = NULL;

        http_headers = curl_slist_append(http_headers, CONTENT_TYPE_JSON);


        /*  Specify URL to get
         *  Specify the user using the token key
         *  Specify that we are going to post
         *  Specify the data we are about to send
         *  Specify the HTTP header
         *  Send all data to the WriteMemoryCallback method
         */
        curl_easy_setopt(s, CURLOPT_URL, url_request);
        curl_easy_setopt(s, CURLOPT_USERPWD, pb_config_get_token_key(p_config));
        curl_easy_setopt(s, CURLOPT_PROXY, pb_config_get_proxy(p_config) );
        curl_easy_setopt(s, CURLOPT_TIMEOUT, pb_config_get_timeout(p_config) );
        curl_easy_setopt(s, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(s, CURLOPT_HTTPHEADER, http_headers);
        curl_easy_setopt(s, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(s, CURLOPT_WRITEDATA, (void *) &ms);


        /* Get data
         */
        r = curl_easy_perform(s);
        curl_easy_getinfo(s, CURLINFO_RESPONSE_CODE, &http_code);


        /* Checking errors
         */
        if ( r != CURLE_OK )
        {
            eprintf("curl_easy_perform() failed: %s", curl_easy_strerror(r) );
        }

        curl_easy_cleanup(s);
        curl_slist_free_all(http_headers);

        // Copy the data before removing them.
        if ( ms.data )
        {
            *length = ms.size;
            memcpy(result, ms.data, ms.size);
            free(ms.data);
        }

        #ifdef __TRACES__
        if ( http_code == HTTP_OK )
        {
            gprintf("\e[37m %s %u\e[0m %s", url_request, http_code, result);
        }
        else
        {
            eprintf("\e[37m %s %u\e[0m %s", url_request, http_code, result);
        }
        #endif
    }

    return (http_code);
}


http_code_t pb_requests_post_multipart(char              *result,
                        size_t            *length, 
                        const char        *url_request,
                        const pb_config_t *p_config,
                        const pb_file_t   *file
                        )
{
    /*  Documentation on CURL for C can be found at http://curl.haxx.se/libcurl/c/
     */


    unsigned short              http_code   = 0;
    struct memory_struct_s      ms          = {0};


    /*  Start a libcurl easy session
     */
    CURL            *s;
    CURLcode        r = CURLE_OK;

    struct curl_httppost        *formpost       = NULL;
    struct curl_httppost        *lastptr        = NULL;
    struct curl_slist           *http_headers   = NULL;


    curl_global_init(CURL_GLOBAL_ALL);


    /* Fill in the file upload field
     */
    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "file", CURLFORM_FILE, pb_file_get_filepath(file), CURLFORM_END);


    /* Initialize the session
     */
    s = curl_easy_init();

    if ( ! s )
    {
        eprintf("curl_easy_init() could not be initiated.\n");
    }
    else
    {
        http_headers = curl_slist_append(http_headers, CONTENT_TYPE_MULTIPART);
        curl_easy_setopt(s, CURLOPT_USERPWD, pb_config_get_token_key(p_config));
        curl_easy_setopt(s, CURLOPT_PROXY, pb_config_get_proxy(p_config) );
        curl_easy_setopt(s, CURLOPT_TIMEOUT, pb_config_get_timeout(p_config) );
        curl_easy_setopt(s, CURLOPT_URL, url_request);
        curl_easy_setopt(s, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(s, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(s, CURLOPT_WRITEDATA, (void *) &ms);


        /* Get data
         */
        r = curl_easy_perform(s);
        curl_easy_getinfo(s, CURLINFO_RESPONSE_CODE, &http_code);


        /* Checking errors
         */
        if ( r != CURLE_OK )
        {
            eprintf("curl_easy_perform() failed: %s", curl_easy_strerror(r) );

            return (http_code);
        }

        curl_easy_cleanup(s);
        curl_formfree(formpost);
        curl_slist_free_all(http_headers);

        // Copy the data before removing them.
        if ( ms.data )
        {
            *length = ms.size;
            memcpy(result, ms.data, ms.size);
            free(ms.data);
        }

        #ifdef __TRACES__
        if ( http_code == HTTP_OK )
        {
            gprintf("\e[37m %s %u\e[0m %s", url_request, http_code, result);
        }
        else
        {
            eprintf("\e[37m %s %u\e[0m %s", url_request, http_code, result);
        }
        #endif
    }

    return (http_code);
}


http_code_t pb_requests_delete(char               *result,
                         size_t             *length,
                         const char         *url_request,
                         const pb_config_t *p_config
                         )
{
    /*  Documentation on CURL for C can be found at http://curl.haxx.se/libcurl/c/
     */
    unsigned short              http_code   = HTTP_UNKNOWN_CODE;
    struct memory_struct_s      ms          = {0};


    /*  Start a libcurl easy session
     */
    CURL     *s = curl_easy_init();


    if ( ! s )
    {
        eprintf("curl_easy_init() could not be initiated.\n");
    }
    else
    {
        CURLcode     r = CURLE_OK;
        struct curl_slist     *http_headers = NULL;

        http_headers = curl_slist_append(http_headers, CONTENT_TYPE_JSON);


        /*  Specify URL to get
         *  Specify the user using the token key
         *  Specify the HTTP header
         */
        curl_easy_setopt(s, CURLOPT_URL, url_request);
        curl_easy_setopt(s, CURLOPT_USERPWD, pb_config_get_token_key(p_config));
        curl_easy_setopt(s, CURLOPT_PROXY, pb_config_get_proxy(p_config) );
        curl_easy_setopt(s, CURLOPT_TIMEOUT, pb_config_get_timeout(p_config) );
        curl_easy_setopt(s, CURLOPT_HTTPHEADER, http_headers);
        curl_easy_setopt(s, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(s, CURLOPT_WRITEDATA, (void *) &ms);


        /* Set the DELETE command
         */
        curl_easy_setopt(s, CURLOPT_CUSTOMREQUEST, "DELETE");


        /* Get data
         */
        r = curl_easy_perform(s);
        curl_easy_getinfo(s, CURLINFO_RESPONSE_CODE, &http_code);


        /* Checking errors
         */
        if ( r != CURLE_OK )
        {
            eprintf("curl_easy_perform() failed: %s", curl_easy_strerror(r) );
        }

        curl_easy_cleanup(s);
        curl_slist_free_all(http_headers);

        // Copy the data before removing them.
        if ( ms.data )
        {
            *length = ms.size;
            memcpy(result, ms.data, ms.size);
            free(ms.data);
        }

        #ifdef __TRACES__
        if ( http_code == HTTP_OK )
        {
            gprintf("\e[37m %s %u\e[0m %s", url_request, http_code, result);
        }
        else
        {
            eprintf("\e[37m %s %u\e[0m %s", url_request, http_code, result);
        }
        #endif
    }

    return (http_code);
}



/**
 * @brief Write a downloaded element in the memory
 *
 * @param contents Downloaded content
 * @param size Size of the buffer
 * @param nmemb Size of each element of that buffer
 * @param userdata The pointer to the memory
 *
 * @return Return the size of the downloaded chunk
 */
static size_t write_memory_callback(void    *contents,
                                    size_t  size,
                                    size_t  nmemb,
                                    void    *userdata
                                    )
{
    size_t realsize            = size * nmemb;
    struct memory_struct_s *ms = (struct memory_struct_s *) userdata;

    // Resize the buffer to hold the old data + the new data.
    ms->data = realloc(ms->data, ms->size + realsize + 1);


    if ( ms->data == NULL )
    {
        // Out of memory!
        eprintf("Not enough memory (realloc returned NULL)\n");

        realsize = 0;
    }
    else
    {
        // Copy the new data into the buffer.
        memcpy(ms->data + ms->size, contents, realsize);

        // Update the size of the buffer.
        ms->size += realsize;

        // Null terminate the buffer/string.
        ms->data[ms->size] = 0;
    }

    return (realsize);
}
