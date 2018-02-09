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
    char* proxy;             ///< HTTP/HTTPS proxy
    long  timeout;             ///< CURL timeout
    char* token_key;             ///< Pushbullet token key
    pthread_mutex_t mtx;        /// Muxtex for THREAD-SAFE
    int   ref;          ///< Reference count
} pb_config_t;


#ifdef __cplusplus
}
#endif


#endif // __PB_CONFIG_PRIV__