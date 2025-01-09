#include <gtest/gtest.h>

#include "scran_tests/initial_value.hpp"

TEST(InitialValue, Basic) {
    int last = scran_tests::initial_value();
    for (int i = 0; i < 1000; ++i) {
        auto val = scran_tests::initial_value();
        EXPECT_GE(val, 1);
        EXPECT_LE(val, 255);
        EXPECT_NE(last, val);
        last = val;
    }
}
