/**
 * @file requests.c
 * @author hbuyse
 * @date 08/05/2016
 */

#include <stdio.h>          // fprintf, stderr
#include <stdlib.h>          // realloc, free
#include <string.h>          // memcpy
#include <curl/curl.h>          // CURL, CURLcode, struct curl_slist, curl_slist_append, curl_easy_init,
                                // curl_easy_setopt, curl_easy_perform, curl_easy_cleanup, curl_slist_free_all


/**
 * \struct Memory_struct_s
 * \brief      Chunk of memory used by write_memory_callback.
 * \details    It stores its data and its size.
 */
struct Memory_struct_s {
    char *memory;       ///< Pointer to the memory
    size_t size;        ///< Size of the memory allocated
};


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
static size_t write_memory_callback(void    *contents,
                                    size_t  size,
                                    size_t  nmemb,
                                    void    *userp
                                    )
{
    size_t     realsize = size * nmemb;
    struct Memory_struct_s     *mem = (struct Memory_struct_s *) userp;


    // Resize the buffer to hold the old data + the new data.
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);

    if ( mem->memory == NULL )
    {
        // Out of memory!
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");

        return (0);
    }


    // Copy the new data into the buffer.
    memcpy(&(mem->memory[mem->size]), contents, realsize);


    // Update the size of the buffer.
    mem->size += realsize;


    // Null terminate the buffer/string.
    mem->memory[mem->size] = 0;

    return (realsize);
}



unsigned short pb_get(char  *result,
                      char  *url_request,
                      char  *token_key
                      )
{
    /*  Documentation on CURL for C can be found at http://curl.haxx.se/libcurl/c/
     */
    unsigned short              http_code   = 0;
    struct Memory_struct_s      ms          =
    {
        .memory = (char *) calloc(1, sizeof(char) ),
        .size   = 0
    };


    /*  Start a libcurl easy session
     */
    CURL     *s = curl_easy_init();


    if ( s )
    {
        CURLcode     r = CURLE_OK;
        struct curl_slist     *http_headers = NULL;

        http_headers = curl_slist_append(http_headers, "Content-Type: application/json");


        /*  Specify URL to get
         *  Specify the user using the token key
         *  Specify the data we are about to send
         *  Specify the HTTP header
         *  Send all data to the write_memory_callback method
         */
        curl_easy_setopt(s, CURLOPT_URL, url_request);
        curl_easy_setopt(s, CURLOPT_USERPWD, token_key);
        curl_easy_setopt(s, CURLOPT_HTTPHEADER, http_headers);
        curl_easy_setopt(s, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(s, CURLOPT_WRITEDATA, (void *) &ms);


        /* Get data && http status code
         */
        r = curl_easy_perform(s);
        curl_easy_getinfo(s, CURLINFO_RESPONSE_CODE, &http_code);


        /* Checking errors
         */
        if ( r != CURLE_OK )
        {
            #ifdef __DEBUG__
            fprintf(stderr, "\e[1;32m[%s]\e[0m curl_easy_perform() failed: %s", __func__, curl_easy_strerror(r) );
            #endif

            return (http_code);
        }

        curl_easy_cleanup(s);
        curl_slist_free_all(http_headers);
    }
    else
    {
        #ifdef __DEBUG__
        fprintf(stderr, "\e[1;32m[%s]\e[0m curl_easy_init() could not be initiated.", __func__);
        #endif

        return (0);
    }


    // Copy the data before removing them.
    if ( ms.memory )
    {
        memcpy(result, ms.memory, ms.size);
        free(ms.memory);
    }

    return (http_code);
}



unsigned short pb_post(char *result,
                       char *url_request,
                       char *token_key,
                       char *data
                       )
{
    /*  Documentation on CURL for C can be found at http://curl.haxx.se/libcurl/c/
     */
    unsigned short              http_code   = 0;
    struct Memory_struct_s      ms          =
    {
        .memory = (char *) calloc(1, sizeof(char) ),
        .size   = 0
    };


    /*  Start a libcurl easy session
     */
    CURL     *s = curl_easy_init();


    if ( s )
    {
        CURLcode     r = CURLE_OK;
        struct curl_slist     *http_headers = NULL;

        http_headers = curl_slist_append(http_headers, "Content-Type: application/json");


        /*  Specify URL to get
         *  Specify the user using the token key
         *  Specify that we are going to post
         *  Specify the data we are about to send
         *  Specify the HTTP header
         *  Send all data to the WriteMemoryCallback method
         */
        curl_easy_setopt(s, CURLOPT_URL, url_request);
        curl_easy_setopt(s, CURLOPT_USERPWD, token_key);
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
            #ifdef __DEBUG__
            fprintf(stderr, "\e[1;32m[%s]\e[0m curl_easy_perform() failed: %s", __func__, curl_easy_strerror(r) );
            #endif

            return (http_code);
        }

        curl_easy_cleanup(s);
        curl_slist_free_all(http_headers);
    }
    else
    {
        #ifdef __DEBUG__
        fprintf(stderr, "\e[1;32m[%s]\e[0m curl_easy_init() could not be initiated.", __func__);
        #endif

        return (0);
    }


    // Copy the data before removing them.
    if ( ms.memory )
    {
        memcpy(result, ms.memory, ms.size);
        free(ms.memory);
    }

    return (http_code);
}



unsigned short pb_delete(char   *result,
                         char   *url_request,
                         char   *token_key
                         )
{
    /*  Documentation on CURL for C can be found at http://curl.haxx.se/libcurl/c/
     */
    unsigned short              http_code   = 0;
    struct Memory_struct_s      ms          =
    {
        .memory = (char *) calloc(1, sizeof(char) ),
        .size   = 0
    };


    /*  Start a libcurl easy session
     */
    CURL     *s = curl_easy_init();


    if ( s )
    {
        CURLcode     r = CURLE_OK;
        struct curl_slist     *http_headers = NULL;

        http_headers = curl_slist_append(http_headers, "Content-Type: application/json");


        /*  Specify URL to get
         *  Specify the user using the token key
         *  Specify the HTTP header
         */
        curl_easy_setopt(s, CURLOPT_URL, url_request);
        curl_easy_setopt(s, CURLOPT_USERPWD, token_key);
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
            #ifdef __DEBUG__
            fprintf(stderr, "\e[1;32m[%s]\e[0m curl_easy_perform() failed: %s", __func__, curl_easy_strerror(r) );
            #endif

            return (http_code);
        }

        curl_easy_cleanup(s);
        curl_slist_free_all(http_headers);
    }
    else
    {
        #ifdef __DEBUG__
        fprintf(stderr, "\e[1;32m[%s]\e[0m curl_easy_init() could not be initiated.", __func__);
        #endif

        return (0);
    }


    // Copy the data before removing them.
    if ( ms.memory )
    {
        memcpy(result, ms.memory, ms.size);
        free(ms.memory);
    }

    return (http_code);
}