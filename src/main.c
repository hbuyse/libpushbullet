/**
 * @file: main.c
 * @author: hbuyse
 */

#include <user.h>
#include <http_code.h>

int main(void)
{
    PB_user_t           user;
    unsigned char       res = 0;


    res = pb_get_user(&user, "0000000000000000000000000000000000");

    if ( res == HTTP_OK )
    {
        dump_user_info(user);
    }

    return (0);
}