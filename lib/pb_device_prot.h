#ifndef __PB_DEVICE_PROT__
#define __PB_DEVICE_PROT__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @enum pb_device_icon
 * @brief Device's icon
 */
typedef enum {
    ICON_UNKNOWN = -1,          ///< Unknown
    ICON_DESKTOP = 0,          ///< Desktop
    ICON_BROWSER,          ///< Browser
    ICON_WEBSITE,          ///< Website
    ICON_LAPTOP,          ///< Laptop
    ICON_TABLET,          ///< Tablet
    ICON_PHONE,          ///< Phone
    ICON_WATCH,          ///< Watch
    ICON_SYSTEM,          ///< System
    ICON_DEVICE          ///< Device
} pb_device_icon;


typedef struct pb_device_s pb_device_t;


int pb_device_set_type(pb_device_t* p_device, pb_device_icon type);

pb_device_icon pb_device_get_type(const pb_device_t* p_device);

int pb_device_set_next(pb_device_t* p_device, pb_device_t* p_next);

pb_device_t* pb_device_get_next(const pb_device_t* p_device);

char* pb_device_get_iden(const pb_device_t* p_device);

double pb_device_get_created(const pb_device_t* p_device);

double pb_device_get_modified(const pb_device_t* p_device);

char* pb_device_get_nickname(const pb_device_t* p_device);

char* pb_device_get_manufacturer(const pb_device_t* p_device);

char* pb_device_get_model(const pb_device_t* p_device);

short pb_device_get_app_version(const pb_device_t* p_device);

char* pb_device_get_icon(const pb_device_t* p_device);

/**
 * @brief      Fill the device structure with the informations contained in the JSON object
 *
 * @param[out]  browser  The browser structure we fill
 * @param[in]  json_obj  The JSON object
 */
void pb_device_fill_from_json(JsonObject *object, const gchar *member_name, JsonNode *member_node, gpointer userdata);

#ifndef NDEBUG
/**
 * @brief      Display on stdout the informations about the device
 *
 * @param[in]  p_device  Pointer to the device
 */
void pb_device_dump_infos(const pb_device_t* p_device);
#endif

#ifdef __cplusplus
}
#endif

#endif // __PB_DEVICE_PROT__
