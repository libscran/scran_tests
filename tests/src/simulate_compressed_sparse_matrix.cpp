#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>

#include "scran_tests/simulate_compressed_sparse_matrix.hpp"

TEST(SimulateCompressedSparseMatrix, Basic) {
    {
        scran_tests::SimulateCompressedSparseMatrixParameters params;
        auto res = scran_tests::simulate_compressed_sparse_matrix(10, 50, params);
        EXPECT_EQ(res.primary, 10);
        EXPECT_EQ(res.secondary, 50);

        EXPECT_EQ(res.pointers.size(), 11);
        EXPECT_EQ(res.pointers.front(), 0);
        EXPECT_GT(res.pointers.back(), 0);
        EXPECT_LT(res.pointers.back(), 500);
        EXPECT_TRUE(std::is_sorted(res.pointers.begin(), res.pointers.end()));

        EXPECT_EQ(res.pointers.back(), res.data.size());
        for (auto x : res.data) {
            EXPECT_GE(x, -10);
            EXPECT_LT(x, 10);
        }

        EXPECT_EQ(res.pointers.back(), res.index.size());
        for (int p = 0; p < 10; ++p) {
            const auto start = res.pointers[p], end = res.pointers[p + 1];
            EXPECT_TRUE(std::is_sorted(res.index.begin() + start, res.index.begin() + end));
            for (auto s = start; s < end; ++s) {
                EXPECT_GE(res.index[s], 0);
                EXPECT_LT(res.index[s], 50);
            }
        }

        // confirm that not all values are identical.
        std::sort(res.data.begin(), res.data.end());
        EXPECT_NE(res.data.front(), res.data.back());
        std::sort(res.index.begin(), res.index.end());
        EXPECT_NE(res.index.front(), res.index.back());
    }

    // Bounds make sense.
    {
        scran_tests::SimulateCompressedSparseMatrixParameters params;
        auto res = scran_tests::simulate_compressed_sparse_matrix(90, 20, []{
            scran_tests::SimulateCompressedSparseMatrixParameters params;
            params.lower = 1;
            params.upper = 6.6;
            return params;
        }());

        EXPECT_FALSE(res.data.empty());
        for (auto x : res.data) {
            EXPECT_GE(x, 1);
            EXPECT_LT(x, 6.6);
        }
        std::sort(res.data.begin(), res.data.end());
        EXPECT_NE(res.data.front(), res.data.back());
    }

    // Respects the seed.
    {
        scran_tests::SimulateCompressedSparseMatrixParameters params;
        params.seed = 1;
        auto res = scran_tests::simulate_compressed_sparse_matrix(50, 30, params);
        auto res1 = scran_tests::simulate_compressed_sparse_matrix(50, 30, params);
        EXPECT_EQ(res.data, res1.data);
        EXPECT_EQ(res.index, res1.index);
        EXPECT_EQ(res.pointers, res1.pointers);

        params.seed = 2;
        auto res2 = scran_tests::simulate_compressed_sparse_matrix(50, 30, params);
        EXPECT_NE(res.data, res2.data);
    }
}

TEST(SimulateCompressedSparseMatrix, Unsigned) {
    scran_tests::SimulateCompressedSparseMatrixParameters<uint8_t> params;
    EXPECT_EQ(params.lower, 0);

    auto res = scran_tests::simulate_compressed_sparse_matrix(10, 30, params);
    EXPECT_GT(res.data.size(), 0);
    for (auto x : res.data) {
        EXPECT_LT(x, 10);
    }

    std::sort(res.data.begin(), res.data.end());
    EXPECT_NE(res.data.front(), res.data.back());
}
