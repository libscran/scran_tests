#ifndef SCRAN_TEST_EXPECT_ERROR_H
#define SCRAN_TEST_EXPECT_ERROR_H

#include <gtest/gtest.h>
#include <string>

template<typename Function_>
void expect_error(Function_ fun, std::string match) {
    bool failed = false;
    std::string msg;

    try {
        fun();
    } catch(std::exception& e) {
        failed = true;
        msg = e.what();
    }

    EXPECT_TRUE(failed) << "function did not throw an exception with message '" << match << "'";
    EXPECT_TRUE(msg.find(match) != std::string::npos) << "function did not throw an exception with message '" << match << "'";
}

#endif
