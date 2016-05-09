/**
 * @file devices.h
 * @date 08/05/2016
 *
 * @brief Contains all the functions that reports to the user's devices
 */


#ifndef __DEVICES_H__
#define __DEVICES_H__

#include <json/json.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    unsigned char active;          // true,
    char *iden;          // "ujEIL5AaxhYsjAyl73vD3s",
    double created;          // 1459865409.299245,
    double modified;          // 1459865409.2992482,
    char *type;          // "android",
    char *kind;          // "android",
    char *nickname;          // "LGE Nexus 5X",
    unsigned char generated_nickname;          // true,
    char *manufacturer;          // "LGE",
    char *model;          // "Nexus 5X",
    short app_version;          // 256,
    json_object *fingerprint;          // "{\"android_id\":\"f48298904b847bd0\"}",
    char *push_token;          // "fcH5ndd7TuY:APA91bHaNqTASWFRP7dOg5YUfEhuTNv1AB-zBljo_mHzB-zUtUxAPtYkEUwKz69V0dG5vWJFhUb52x7sbEwCZbAYjxYpmKMbacfEtMi1Dvm-eqdKWzJEJQzYWTD5lyADztPcFHIRYYEQ",
    unsigned char pushable;          // true,
    unsigned char has_sms;          // true,
    unsigned char has_mms;          // true,
    char *icon;          // "phone",
    char *remote_files;          // "disabled"
} PB_phone_t;


typedef struct {
    unsigned char active;          // true,
    char *iden;          // "ujEIL5AaxhYsjAaqJB5LXg",
    double created;          // 1427959507.662864,
    double modified;          // 1427959507.662869,
    char *type;          // "chrome",
    char *kind;          // "chrome",
    char *nickname;          // "Chrome",
    char *manufacturer;          // "Google",
    char *model;          // "Chrome",
    short app_version;          // 188,
    unsigned char pushable;          // true,
    char *icon;          // "browser"
} PB_browser_t;

unsigned short pb_get_devices(PB_user_t user);


#ifdef __cplusplus
}
#endif

#endif          // __DEVICES_H__