/**
 * @file user.h
 * @date 08/05/2016
 *
 * @brief Contains all the functions that have a report with the user informations
 */


#ifndef __USER_H__
#define __USER_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \struct PB_user_t
 * \brief Contains the user informations.
 */
typedef struct {
    char *token_key;          ///< The user's token_key
    unsigned char active;          ///< Boolean that tells if the user is active or not
    double created;
    double modified;
    const char *email;          ///< The user's email
    const char *email_normalized;          ///< The user's email normalized
    const char *iden;           ///< The user's identification
    const char *image_url;          ///< The URL to the user's photo
    const char *name;           ///< The user's name
    int max_upload_size;
    void *devices;          ///< The list of active devices
} PB_user_t;


/**
 * \brief      Download the user's informations from PushBullet using its API
 *
 * \param      user_to_fill  The user structure that we fill
 * \param      token_key     The user's token key
 *
 * \return     HTTP status code
 */
unsigned char pb_get_user_info(PB_user_t *user_to_fill, char *token_key);


#ifdef __cplusplus
}
#endif

#endif          // __USER_H__