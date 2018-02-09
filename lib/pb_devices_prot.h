#ifndef __PB_DEVICES_PROT__
#define __PB_DEVICES_PROT__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pb_device_s pb_device_t;
typedef struct pb_devices_s pb_devices_t;



/**
 * @brief      Get the number of active devices of the given user
 *
 * @param[in]  user  The user
 *
 * @return     Number of active devices
 */
ssize_t pb_devices_get_number_active(const pb_devices_t *p_devices);

pb_device_t* pb_devices_get_list(const pb_devices_t* p_devices);

int pb_devices_get_ref(const pb_devices_t* p_devices);

int pb_devices_load_devices_from_data(pb_devices_t* p_devices, char* result, size_t result_sz);

int pb_devices_add_new_device(pb_devices_t* p_devices, pb_device_t* p_new_device);


#ifdef __cplusplus
}
#endif

#endif // __PB_DEVICES_PROT__
