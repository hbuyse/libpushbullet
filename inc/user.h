/**
 * @file user.h
 * @author hbuyse
 * @date 08/05/2016
 *
 * @brief  Functions that have a report with the user informations
 */


#ifndef __USER_H__
#define __USER_H__

#include <pb_structures.h>          // PB_user_t

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief      Download the user's informations from PushBullet using its API
 *
 * \param      user_to_fill  The user structure that we fill
 * \param      token_key     The user's token key
 *
 * \return     HTTP status code
 */
unsigned short pb_get_user_info(PB_user_t *user_to_fill, char *token_key);


/**
 * \brief      Free the user structure
 *
 * \param      user  The user we want to free
 */
void pb_free_user(PB_user_t *user);


#ifdef __cplusplus
}
#endif

#endif          // __USER_H__