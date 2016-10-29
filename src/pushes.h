/**
 * @file pushes.h
 * @author hbuyse
 * @date 08/05/2016
 *
 * @brief  Functions that reports to the pushes
 */


#ifndef __NOTE_H__
#define __NOTE_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief      Send a note
 *
 * @param[out] result           The buffer where we store the JSON response
 * @param[in]  note             The note's informations (title, body)
 * @param[in]  device_nickname  The device nickname
 * @param[in]  user             The user that sends the note
 *
 * @return     The HTTP status code to the \a pb_post
 */
unsigned short pb_push_note(char *result, const pb_note_t note, const char *device_nickname, const pb_user_t user);


/**
 * @brief      Send a link
 *
 * @param[out] result           The buffer where we store the JSON response
 * @param[in]  link             The link's informations (title, body, URL)
 * @param[in]  device_nickname  The device nickname
 * @param[in]  user             The user that sends the link
 *
 * @return     The HTTP status code to the \a pb_post
 */
unsigned short pb_push_link(char *result, const pb_link_t link, const char *device_nickname, const pb_user_t user);


unsigned short pb_push_file(char *result, pb_file_t *file, const char *device_nickname, const pb_user_t user);


#ifdef __cplusplus
}
#endif

#endif          // __NOTE_H__