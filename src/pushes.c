/**
 * @file pushes.c
 * @date 08/05/2016
 */

#include <stdio.h>      // fprintf, stderr
#include <stdlib.h>     // realloc
#include <string.h>     // memcpy

#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};


/**
 * @brief Write a downlaoded element in the memory
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
    size_t     realsize             = size * nmemb;
    struct MemoryStruct     *mem    = (struct MemoryStruct *) userp;


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