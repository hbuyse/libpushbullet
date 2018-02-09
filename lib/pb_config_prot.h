/**
 * @file pb_config_prot.h
 * @author hbuyse
 * @date 30/01/2018
 */

#ifndef __PB_CONFIG_PROT__
#define __PB_CONFIG_PROT__


#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_PROXY_KEY_ENV  "http_proxy"
#define HTTPS_PROXY_KEY_ENV "proxy"
#define PB_TOKEN_KEY_ENV "PB_TOKEN_KEY"


typedef struct pb_config_s pb_config_t;


int pb_config_get_ref(const pb_config_t* p_config);

int pb_config_lock(pb_config_t* p_config);

int pb_config_unlock(pb_config_t* p_config);

int pb_config_copy(pb_config_t* p_dst, pb_config_t* p_src);

#ifdef __cplusplus
}
#endif


#endif // __PB_CONFIG_PROT__
