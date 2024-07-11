#include <gtest/gtest.h>

#include "scran_tests/vector_n.hpp"

TEST(VectorN, Basic) {
    std::vector<double> whee{1,2,3,4};
    EXPECT_EQ(whee, scran_tests::vector_n(whee.data(), whee.size()));

    auto copy = whee;
    copy.pop_back();
    EXPECT_EQ(copy, scran_tests::vector_n(whee.data(), whee.size() - 1));
}
