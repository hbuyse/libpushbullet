/**
 * @file: main.c
 * @author: hbuyse
 *
 * @brief Testing program
 */
#include <stdlib.h>
#include <check.h>

extern Suite * pb_config_suite(void);

int main(int    argc,
         char   *argv[]
         )
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = pb_config_suite();
    sr = srunner_create(s);

    // srunner_set_xml (sr, "results.xml");
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}