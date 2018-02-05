/**
 * @file pb_session.c
 * @author hbuyse
 * @date 29/01/2018
 */

#include <curl/curl.h>      // curl_global_init

#include "pb_utils.h" // eprintf


int pb_init(void)
{
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);

    if (res != 0)
    {
        eprintf("Error during global initialization of libcurl.");
    }
    return (int) res;
}

void pb_term(void)
{
    curl_global_cleanup();
}