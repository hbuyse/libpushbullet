#ifndef __PB_USER_PROT__
#define __PB_USER_PROT__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pb_user_s pb_user_t;
typedef struct pb_devices_s pb_devices_t;


int pb_user_get_ref(const pb_user_t *p_user);

/**
 * @brief      Set a new list of devices
 *
 * @param[in]  p_user     The user
 * @param      p_devices  The devices
 *
 * @return     
 */
int pb_user_set_devices(pb_user_t* p_user, pb_devices_t* p_devices);


int pb_user_unref_devices(const pb_user_t *p_user);

#ifdef __cplusplus
}
#endif

#endif // __PB_USER_PROT__
