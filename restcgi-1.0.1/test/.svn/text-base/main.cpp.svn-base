#include "test.h"
#include <iostream>
namespace restcgi_test {
    void date_time_tests(test_utils::test& t);
    void utils_tests(test_utils::test& t);
    void httpsyn_tests(test_utils::test& t);
    void env_tests(test_utils::test& t);
    void ipsyn_tests(test_utils::test& t);
    void uri_tests(test_utils::test& t);
    void cookie_tests(test_utils::test& t);
    void hdr_tests(test_utils::test& t);
    void status_code_tests(test_utils::test& t);
    void method_tests(test_utils::test& t);
    void ctmpl_tests(test_utils::test& t);
    void version_tests(test_utils::test& t);
    void resource_tests(test_utils::test& t);
    inline void setup(test_utils::test& t) {
            date_time_tests(t);
            utils_tests(t);
            httpsyn_tests(t);
            env_tests(t);
            cookie_tests(t);
            hdr_tests(t);
            status_code_tests(t);
            method_tests(t);
            ctmpl_tests(t);
            version_tests(t);
            resource_tests(t);
    }
}
int main(int argc, char** argv) {
    test_utils::test test;
    restcgi_test::setup(test);
    int res = test.run();
    if (res)
        std::cerr << "*** " << test_utils::test::errors() << " test(s) failed" << std::endl;
    else
        std::cerr << "--- no errors" << std::endl;
    return res;
}

