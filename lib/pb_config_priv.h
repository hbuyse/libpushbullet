/**
 * @file pb_config_priv.h
 * @author hbuyse
 * @date 30/01/2018
 */

#ifndef __PB_CONFIG_PRIV__
#define __PB_CONFIG_PRIV__

#define HTTP_PROXY_KEY_ENV  "http_proxy"
#define HTTPS_PROXY_KEY_ENV "proxy"
#define PB_TOKEN_KEY_ENV "PB_TOKEN_KEY"

#ifdef __cplusplus

namespace pushbullet {
    class Config {
    private:
        std::string _proxy;
        long _timeout;
        std::string _token_key;
    public:
        Config(void);
        Config(std::string proxy, long timeout, std::string token_key);
        ~Config(void);

        int set_proxy(std::string proxy);
        int set_timeout(long timeout);
        int set_token_key(std::string proxy);
    };
}

#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct pb_config_s {
    char* proxy;             ///< HTTP/HTTPS proxy
    long  timeout;             ///< CURL timeout
    char* token_key;             ///< Pushbullet token key
    int   ref;          ///< Reference count
} pb_config_t;


#ifdef __cplusplus
}
#endif


#endif // __PB_CONFIG_PRIV__