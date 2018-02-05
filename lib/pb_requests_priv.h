/**
 * @file requests.h
 * @author hbuyse
 * @date 08/05/2016
 *
 * @brief  Functions in order to communicate to the Pushbullet API
 */


#ifndef __REQUESTS_H__
#define __REQUESTS_H__

#include "pushbullet.h"

#ifdef __cplusplus
extern "C" {
#endif


#define CONTENT_TYPE_JSON       "Content-Type: application/json"


#define CONTENT_TYPE_MULTIPART  "Content-Type: multipart/form-data"


#define CURL_USERAGENT "libcurl-agent/1.0"


/**
 * @struct memory_struct_s
 * @brief      Chunk of memory used by write_memory_callback.
 * @details    It stores its data and its size.
 */
struct memory_struct_s {
    char *data;          ///< Pointer to the memory
    size_t size;          ///< Size of the memory allocated
};

#ifdef __cplusplus
}
#endif

#endif          // __REQUESTS_H__