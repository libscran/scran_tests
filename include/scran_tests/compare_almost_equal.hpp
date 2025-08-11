#ifndef SCRAN_TESTS_COMPARE_ALMOST_EQUAL_HPP
#define SCRAN_TESTS_COMPARE_ALMOST_EQUAL_HPP

#include <gtest/gtest.h>
#include <cmath>

/**
 * @file compare_almost_equal.hpp
 * @brief Check for almost-equality of floating-point values.
 */

namespace scran_tests {

/**
 * Check if two floating-point numbers are equal, accounting for some relative difference.
 * Two numbers are considered equal if their difference is less than the product of `tolerance` and their mean.
 * If both numbers are close to zero, they are considered equal if the difference is below 1e-15;
 * this avoids being overly stringent for numbers that should be zero, at the cost of potentially missing large relative differences.
 *
 * @param left One of the numbers.
 * @param right The other number.
 * @param tol The tolerance for the comparison.
 * @param report Whether to report the error via GoogleTest.
 *
 * @return Whether the two numbers are equal.
 */
inline bool compare_almost_equal(double left, double right, double tol = 1e-8, bool report = true) {
    if (left != right) {
        // Use relative tolerance to check that we get a similar float.
        // EXPECT_DOUBLE_EQ and EXPECT_FLOAT_EQ tend to be a bit too stringent
        // when numerical imprecision is expected. We define the relative difference
        // as |difference| / |mean| of the two values, and check that this is 
        // below some acceptable error, in this case 1e-8.
        //
        // It's worth nothing that I spent a long time using EXPECT_FLOAT_EQ
        // when I really should have been using EXPECT_DOUBLE_EQ; this would have
        // implicitly used a relative threshold at the 8th significant figure,
        // which is why it never failed from differences in numerical precision.
        double denom = std::abs(left + right) / 2;
        double threshold = denom * tol;

        // This is effectively refactored to |difference| < threshold. However,
        // if threshold falls below the machine epsilon - usually because
        // |mean| is very small - we're effectively back to an exact equality
        // test, so we need to ensure that the threshold doesn't get too small.
        //
        // This compromises the stringency of the test at very-near-zero values,
        // but this could also be viewed as a feature, because we DON'T want to
        // fail the test if one value is zero and another is slightly non-zero
        // (e.g., when something should, in theory, perfectly cancel out).
        if (threshold < 1e-15) { 
            threshold = 1e-15;
        }
        if (std::abs(left - right) > threshold) {
            if (report) {
                EXPECT_TRUE(false) << "mismatch in almost-equal floats (expected " << left << ", got " << right << ")";
            }
            return false;
        }
    }

    return true;
}

/**
 * Check if two vectors contain an equal number of almost-equal floats.
 * This compares the corresponding elements from each vector using `compare_almost_equal()`.
 * Any test failure is reported via GoogleTest.
 *
 * @tparam Vector_ Some vector-like container of floating-point values.
 * 
 * @param left One of the vectors.
 * @param right The other vector.
 * @param tol The tolerance for the comparison, passed to the other `compare_almost_equal()` function.
 */
template<class Vector_>
void compare_almost_equal(const Vector_& left, const Vector_& right, double tol = 1e-8) {
    auto n = left.size();
    ASSERT_EQ(n, right.size());
    for (decltype(n) i = 0; i < n; ++i) {
        if (!compare_almost_equal(left[i], right[i], tol, false)) {
            EXPECT_TRUE(false) << "mismatch in almost-equal floats at element " << i << " (expected " << left[i] << ", got " << right[i] << ")";
            return;
        }
    }
}

}

#endif
