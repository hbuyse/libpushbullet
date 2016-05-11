/**
 * @file devices.h
 * @date 08/05/2016
 *
 * @brief Contains all the functions that reports to the user's devices
 */


#ifndef __DEVICES_H__
#define __DEVICES_H__

#include <pb_structures.h>          // PB_user_t

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief      Get the devices informations and stores it int a linked list in the user structure
 *
 * \param[in]  user  The user in which we store the devices
 *
 * \return     Error code
 */
unsigned short pb_get_devices(PB_user_t *user);


#ifdef __cplusplus
}
#endif

#endif          // __DEVICES_H__