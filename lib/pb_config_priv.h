/**
 * @file pb_config_priv.h
 * @author hbuyse
 * @date 30/01/2018
 */

#ifndef __PB_CONFIG_PRIV__
#define __PB_CONFIG_PRIV__


#ifdef __cplusplus
extern "C" {
#endif


typedef struct pb_config_s {
    char* https_proxy;             ///< HTTPS proxy
    long  timeout;             ///< CURL timeout
    char* token_key;             ///< Pushbullet token key
    int   ref;          ///< Reference count
} pb_config_t;


#ifdef __cplusplus
}
#endif


#endif // __PB_CONFIG_PRIV__