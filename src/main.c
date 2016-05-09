/**
 * @file: main.c
 * @author: hbuyse
 */

#include <user.h>          // pb_get_user_info
#include <devices.h>          // pb_get_devices
#include <http_code.h>          // HTTP_OK

// TOKEN_KEY

int main(void)
{
    PB_user_t           user;
    unsigned char       res = 0;


    #ifdef TOKEN_KEY
    res = pb_get_user_info(&user, TOKEN_KEY);
    #else
    res = pb_get_user_info(&user, "0000000000000000000000000000000000");
    #endif

    if ( res == HTTP_OK )
    {
        dump_user_info(user);
    }

    res = pb_get_devices(user);

    return (0);
}