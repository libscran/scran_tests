#ifndef SCRAN_TESTS_EXPECT_ERROR_HPP
#define SCRAN_TESTS_EXPECT_ERROR_HPP

#include <gtest/gtest.h>
#include <string>

/**
 * @file expect_error.hpp
 * @brief Expect an error to be raised.
 */

namespace scran_tests {

/**
 * Expect an error to be raised with the given message.
 * If no error is raised, or if the error message does not match `match`, an error is reported via GoogleTest.
 *
 * @tparam Function_ Function to call with no arguments.
 *
 * @param fun Function that should raise an error.
 * @param match String to be matched against the error message.
 */
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

    ASSERT_TRUE(failed) << "function did not throw an exception with message '" << match << "'";
    ASSERT_TRUE(msg.find(match) != std::string::npos) << "function did not throw an exception with message '" << match << "'";
}

}

#endif
