/**
 * @file: main.c
 * @author: hbuyse
 */

#include <user.h>          // pb_get_user_info
#include <devices.h>          // pb_get_devices
#include <http_code.h>          // HTTP_OK

// TOKEN_KEY
#ifndef TOKEN_KEY
#define TOKEN_KEY "0000000000000000000000000000000000"
#endif

int main(void)
{
    PB_user_t           user;
    unsigned char       res = 0;


    res = pb_get_user_info(&user, TOKEN_KEY);

    if ( res == HTTP_OK )
    {
        dump_user_info(user);
    }

    res = pb_get_devices(user);

    return (0);
}