/**
 * @file: main.c
 * @author: hbuyse
 *
 * @brief Testing program for user functions
 */

#include <unistd.h>          // getopt, opterr, optarg, optopt, optind
#include <getopt.h>          // struct option
#include <stdlib.h>             // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>          // strcmp
#include <stdio.h>          // fprintf

#include <pushbullet/structures.h>          // pb_user_t, pb_device_t, pb_browser_t, pb_phone_t, ICON_PHONE,
                                            // ICON_BROWSER
#include <pushbullet/user.h>          // pb_get_user_info, pb_free_user
#include <pushbullet/devices.h>          // pb_get_devices
#include <pushbullet/http_code.h>          // HTTP_OK
#include <pushbullet/pushes.h>          // pb_push_note, pb_push_link

#include "../minunit/minunit.h"


/**
 * \brief HTTP proxy in config file
 */
#define     HTTP_PROXY "172.25.0.3:8080"


/**
 * \brief HTTPS proxy in config file
 */
#define     HTTPS_PROXY "172.25.0.3:8080"


/**
 * \brief CURL timeout in config file
 */
#define     CURL_TIMEOUT 2


/**
 * \brief Token key in config file
 */
#define     TOKEN_KEY "toto_is_in_da_place!"



/**
 * \brief Stores the token key given as option
 */
static char             *token_key = NULL;
static pb_user_t        user;

MU_TEST(test_get_config_json)
{
    json_object     *config = NULL;


    // No path given
    config  = pb_get_config_json(NULL);
    mu_check(config == NULL);


    // Open directory
    config  = pb_get_config_json("tests/conf/");
    mu_check(config == NULL);


    // Open non existing file
    config  = pb_get_config_json("tests/conf/null.json");
    mu_check(config == NULL);


    // Open existing but empty file
    config  = pb_get_config_json("tests/conf/empty.json");
    mu_check(config == NULL);


    // Open existing but empty JSON file
    config  = pb_get_config_json("tests/conf/empty_json.json");
    mu_check(config == NULL);


    // Open JSON File
    config  = pb_get_config_json("tests/conf/all.json");
    mu_check(config != NULL);
}


MU_TEST(test_get_http_proxy)
{
    json_object     *config     = NULL;
    const char      *http_proxy = NULL;


    // No path given
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy == NULL);


    // Open directory
    config      = pb_get_config_json("tests/conf/");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/null.json");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty.json");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty_json.json");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/curl_timeout.json");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/token_key.json");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty_values.json");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/proxies.json");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy != NULL);
    mu_check(strcmp(http_proxy, HTTP_PROXY) == 0);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/all.json");
    user.config = config;
    http_proxy  = pb_get_http_proxy(user);
    mu_check(http_proxy != NULL);
    mu_check(strcmp(http_proxy, HTTP_PROXY) == 0);
}


MU_TEST(test_get_https_proxy)
{
    json_object     *config         = NULL;
    const char      *https_proxy    = NULL;


    // No path given
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy == NULL);


    // Open directory
    config      = pb_get_config_json("tests/conf/");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/null.json");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty.json");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty_json.json");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/curl_timeout.json");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/token_key.json");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty_values.json");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/proxies.json");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy != NULL);
    mu_check(strcmp(https_proxy, HTTPS_PROXY) == 0);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/all.json");
    user.config = config;
    https_proxy = pb_get_https_proxy(user);
    mu_check(https_proxy != NULL);
    mu_check(strcmp(https_proxy, HTTPS_PROXY) == 0);
}


MU_TEST(test_get_curl_timeout)
{
    json_object     *config         = NULL;
    int             curl_timeout    = 0;


    // No path given
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == 0);


    // Open directory
    config          = pb_get_config_json("tests/conf/");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == 0);


    // Open JSON File
    config          = pb_get_config_json("tests/conf/null.json");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == 0);


    // Open JSON File
    config          = pb_get_config_json("tests/conf/empty.json");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == 0);


    // Open JSON File
    config          = pb_get_config_json("tests/conf/empty_json.json");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == 0);


    // Open JSON File
    config          = pb_get_config_json("tests/conf/proxies.json");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == 0);


    // Open JSON File
    config          = pb_get_config_json("tests/conf/token_key.json");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == 0);


    // Open JSON File
    config          = pb_get_config_json("tests/conf/empty_values.json");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == 0);


    // Open JSON File
    config          = pb_get_config_json("tests/conf/curl_timeout.json");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == CURL_TIMEOUT);


    // Open JSON File
    config          = pb_get_config_json("tests/conf/all.json");
    user.config     = config;
    curl_timeout    = pb_get_curl_timeout(user);
    mu_check(curl_timeout == CURL_TIMEOUT);
}


MU_TEST(test_get_token_key)
{
    json_object     *config     = NULL;
    const char      *token_key  = NULL;


    // No path given
    token_key   = pb_get_token_key(NULL);
    mu_check(token_key == NULL);


    // Open directory
    config      = pb_get_config_json("tests/conf/");
    token_key   = pb_get_token_key(config);
    mu_check(token_key == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/null.json");
    token_key   = pb_get_token_key(config);
    mu_check(token_key == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty.json");
    token_key   = pb_get_token_key(config);
    mu_check(token_key == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty_json.json");
    token_key   = pb_get_token_key(config);
    mu_check(token_key == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/curl_timeout.json");
    token_key   = pb_get_token_key(config);
    mu_check(token_key == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/proxies.json");
    token_key   = pb_get_token_key(config);
    mu_check(token_key == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/empty_values.json");
    token_key   = pb_get_token_key(config);
    mu_check(token_key == NULL);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/token_key.json");
    token_key   = pb_get_token_key(config);
    mu_check(token_key != NULL);
    mu_check(strcmp(token_key, TOKEN_KEY) == 0);


    // Open JSON File
    config      = pb_get_config_json("tests/conf/all.json");
    token_key   = pb_get_token_key(config);
    mu_check(token_key != NULL);
    mu_check(strcmp(token_key, TOKEN_KEY) == 0);
}


MU_TEST_SUITE(test_config)
{
    // Test
    fprintf(stdout, "\n\e[1m[test_get_config_json]\e[0m\t");
    MU_RUN_TEST(test_get_config_json);
    fprintf(stdout, "\n\e[1m[test_get_http_proxy]\e[0m\t");
    MU_RUN_TEST(test_get_http_proxy);
    fprintf(stdout, "\n\e[1m[test_get_https_proxy]\e[0m\t");
    MU_RUN_TEST(test_get_https_proxy);
    fprintf(stdout, "\n\e[1m[test_get_curl_timeout]\e[0m\t");
    MU_RUN_TEST(test_get_curl_timeout);
    fprintf(stdout, "\n\e[1m[test_get_token_key]\e[0m\t");
    MU_RUN_TEST(test_get_token_key);
}


unsigned char main_config(const char *tk)
{
    token_key = (char *) tk;


    // Launch the test suites
    MU_RUN_SUITE(test_config);


    // Print the report
    MU_REPORT();

    return ( (minunit_fail == 0) ? 0 : 1);
}