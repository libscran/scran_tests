#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>

#include "scran_tests/simulate_vector.hpp"

TEST(SimulateVector, Dense) {
    {
        scran_tests::SimulateVectorParameters params;
        auto res = scran_tests::simulate_vector(100, params);
        EXPECT_EQ(res.size(), 100);
        for (auto x : res) {
            EXPECT_GE(x, -10);
            EXPECT_LT(x, 10);
        }
        std::sort(res.begin(), res.end());
        EXPECT_NE(res.front(), res.back()); // confirm that not all values are identical.
    }

    // Bounds make sense.
    {
        auto res = scran_tests::simulate_vector(100, []{
            scran_tests::SimulateVectorParameters params;
            params.lower = 1;
            params.upper = 6.6;
            return params;
        }());

        EXPECT_EQ(res.size(), 100);
        for (auto x : res) {
            EXPECT_GE(x, 1);
            EXPECT_LT(x, 6.6);
        }
        std::sort(res.begin(), res.end());
        EXPECT_NE(res.front(), res.back());
    }

    // Respects the seed.
    {
        scran_tests::SimulateVectorParameters params;
        params.seed = 1;
        auto res = scran_tests::simulate_vector(100, params);
        auto res1 = scran_tests::simulate_vector(100, params);
        EXPECT_EQ(res, res1);

        params.seed = 2;
        auto res2 = scran_tests::simulate_vector(100, params);
        EXPECT_NE(res, res2);
    }
}

TEST(SimulateVector, DenseUnsigned) {
    scran_tests::SimulateVectorParameters<uint8_t> params;
    EXPECT_EQ(params.lower, 0);

    auto res = scran_tests::simulate_vector(100, params);
    EXPECT_EQ(res.size(), 100);
    for (auto x : res) {
        EXPECT_LT(x, 10);
    }

    std::sort(res.begin(), res.end());
    EXPECT_NE(res.front(), res.back());
}

TEST(SimulateVector, Sparse) {
    scran_tests::SimulateVectorParameters params;
    params.density = 0.1;

    auto res = scran_tests::simulate_vector(1000, params);
    EXPECT_EQ(res.size(), 1000);

    int num_nonzero = 0;
    for (auto x : res) {
        num_nonzero += (x != 0);
    }
    EXPECT_LT(num_nonzero, 200); // should be around 100, so we'll check that it's less than 200.
    EXPECT_GT(num_nonzero, 0); 
}
