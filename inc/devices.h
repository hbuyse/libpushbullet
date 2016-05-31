/**
 * @file devices.h
 * @author hbuyse
 * @date 08/05/2016
 *
 * @brief  Functions that reports to the user's devices
 */


#ifndef __DEVICES_H__
#define __DEVICES_H__

#include <pb_structures.h>          // pb_user_t

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief      Get the devices informations and stores it int a linked list in the user structure
 *
 * \param[in]  user  The user in which we store the devices
 *
 * \return     HTTP status code
 */
unsigned short pb_get_devices(pb_user_t *user);


/**
 * \brief      Clear all the devices list of the given user
 *
 * \param      user  The user's
 */
void pb_free_devices(pb_user_t *user);


/**
 * \brief      Get the number of active devices of the given user
 *
 * \param[in]  user  The user
 *
 * \return     Number of active devices
 */
unsigned char pb_get_number_active_devices(const pb_user_t user);


/**
 * \brief      Get the identification of a device from its name
 *
 * \param[in]  user  The user that have the devices
 * \param[in]  name  The name of the device
 *
 * \return     The device's identification
 */
const char* pb_get_iden_from_name(const pb_user_t user, const char *name);


#ifdef __cplusplus
}
#endif

#endif          // __DEVICES_H__