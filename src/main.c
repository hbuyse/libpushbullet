/**
 * @file: main.c
 * @author: hbuyse
 */

#include <user.h>
#include <http_code.h>

#define TOKEN_KEY "0000000000000000000000000000000000"

int main(void)
{
    PB_user_t           user;
    unsigned char       res = 0;


    res = pb_get_user_info(&user, TOKEN_KEY);

    if ( res == HTTP_OK )
    {
        dump_user_info(user);
    }

    return (0);
}