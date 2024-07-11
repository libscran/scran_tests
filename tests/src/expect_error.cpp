#include <gtest/gtest.h>

#include "scran_tests/expect_error.hpp"

TEST(ExpectError, Basic) {
    scran_tests::expect_error([&]() { throw std::runtime_error("ahh wheeeee"); }, "whee");
}
