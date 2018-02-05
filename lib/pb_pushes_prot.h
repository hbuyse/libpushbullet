/**
 * @file pb_requests_prot.h
 * @author hbuyse
 * @date 30/01/2018
 *
 * @brief  Functions in order to communicate to the Pushbullet API
 */


#ifndef __PB_PUSHES_PROT_H__
#define __PB_PUSHES_PROT_H__


#ifdef __cplusplus
extern "C" {
#endif

typedef struct pb_file_s pb_file_t;

const char* pb_file_get_filepath(const pb_file_t* file);

#ifdef __cplusplus
}
#endif

#endif          // __PB_PUSHES_PROT_H__
