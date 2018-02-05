/**
 * @file pb_user_priv.h
 * @author hbuyse
 * @date 30/01/2018
 */

#ifndef __PB_USER_PRIV__
#define __PB_USER_PRIV__


#ifdef __cplusplus
extern "C" {
#endif


typedef struct pb_config_s pb_config_t;
typedef struct pb_devices_s pb_devices_t;


/**
 * @brief Maximum size of the buffer (4ko - 4096 - 0x1000)
 */
#define     MAX_SIZE_BUF 0x1000


/**
 * @def        JSON_ASSOCIATE_BOOL
 * @def        JSON_ASSOCIATE_STR
 * @def        JSON_ASSOCIATE_INT
 * @def        JSON_ASSOCIATE_DOUBLE
 * @brief      Macro to associate a key in a structure
 *
 * @param      var   The pointer we fill
 * @param      k     The JSON key
 */
#define     JSON_ASSOCIATE_BOOL(var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) var->k = json_node_get_boolean(member_node); } while(0)

#define     JSON_ASSOCIATE_STR(var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) var->k = json_node_dup_string(member_node); } while(0)

#define     JSON_ASSOCIATE_INT(var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) var->k = json_node_get_int(member_node); } while(0)

#define     JSON_ASSOCIATE_DOUBLE(var, k)          \
    do { if ( strcmp(member_name, # k) == 0 ) var->k = json_node_get_double(member_node); } while(0)



/**
 * @struct pb_user_s
 * @brief Contains the user informations.
 */
typedef struct pb_user_s {
    unsigned char active;          ///< Boolean that tells if the user is active or not
    double created;             ///< Epoch time when the user was created
    double modified;            ///< Epoch time when the user was last modified
    char *email;          ///< The user's email
    char *email_normalized;          ///< The user's email normalized
    char *iden;           ///< The user's identification
    char *image_url;          ///< The URL to the user's photo
    char *name;           ///< The user's name
    int max_upload_size;          ///< The maximum size of a file the user can upload in bytes
    pb_config_t *config;            ///< Configuration from the config file
    pb_devices_t *devices;          ///< The list of active devices
    int32_t     ref;          ///< Reference count
} pb_user_t;


#ifdef __cplusplus
}
#endif


#endif // __PB_USER_PRIV__