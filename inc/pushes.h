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
 * \brief      Send a note
 *
 * \param[out] result           The buffer where we store the JSON response
 * \param[in]  title            The note's title
 * \param[in]  body             The note's body
 * \param[in]  device_nickname  The device nickname
 * \param[in]  user             The user that sends the note
 *
 * \return     The HTTP status code to the \a pb_post
 */
unsigned short pb_push_note(char            *result,
                            const char      *title,
                            const char      *body,
                            const char      *device_nickname,
                            const PB_user_t user);


/**
 * \brief      Send a link
 *
 * \param[out] result           The buffer where we store the JSON response
 * \param[in]  title            The link's title
 * \param[in]  body             The link's body
 * \param[in]  device_nickname  The device nickname
 * \param[in]  user             The user that sends the link
 *
 * \return     The HTTP status code to the \a pb_post
 */
unsigned short pb_push_link(char            *result,
                            const char      *title,
                            const char      *body,
                            const char      *url,
                            const char      *device_nickname,
                            const PB_user_t user);


#ifdef __cplusplus
}
#endif

#endif          // __NOTE_H__