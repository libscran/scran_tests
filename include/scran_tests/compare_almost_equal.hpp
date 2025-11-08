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
 * @brief Parameters for `compare_almost_equal()`.
 */
struct CompareAlmostEqualParameters {
    /**
     * Relative tolerance for the difference between values. 
     */
    double relative_tolerance = 1e-8;

    /**
     * Absolute tolerance for the difference between values.
     */
    double absolute_tolerance = 1e-15;

    /**
     * Whether to report a mismatch as a test failure in GoogleTest.
     */
    bool report = true;
};

/**
 * Check if two floating-point numbers are equal, accounting for some relative difference.
 * Two numbers are considered equal if their difference is less than the product of `CompareAlmostEqualParameters::relative_tolerance` and their mean.
 * If both numbers are close to zero, they are considered equal if the difference is below `CompareAlmostEqualParameters::absolute_tolerance`;
 * this avoids being overly stringent for numbers that should be zero, at the cost of potentially missing large relative differences.
 *
 * @param left One of the numbers.
 * @param right The other number.
 * @param params Further parameters.
 *
 * @return Whether the two numbers are equal.
 */
inline bool compare_almost_equal(double left, double right, const CompareAlmostEqualParameters& params) {
    if (left == right) {
        return true;
    }

    // Use relative tolerance to check that we get a similar float.
    // EXPECT_EQ and even EXPECT_DOUBLE_EQ tend to be a bit too stringent when numerical imprecision is expected.
    // We define the relative difference as '|difference| / |mean|' of the two values, and check that this is below some acceptable error, in this case 1e-8.
    //
    // It's worth nothing that I spent a long time using EXPECT_FLOAT_EQ when I really should have been using EXPECT_DOUBLE_EQ.
    // This would have implicitly used a relative threshold at the 8th significant figure, which is why it allowed tests to pass despite imprecision.
    const double denom = std::abs(left + right) / 2;
    double threshold = denom * params.relative_tolerance;

    // 'relative difference < tol' is equivalent to '|difference| < threshold'.
    // However, if '|mean|' is very small, we start to revert back to an exact equality test.
    // This is not ideal - we DON'T want to fail the test if one value is zero and another is very slightly non-zero.
    // So we need to ensure that the threshold doesn't get too small, by setting a lower bound similar to the machine epsilon.
    // (That said, if we're dealing with lots of very-near-zero values, this function won't be appropriate as it'll consider them to be the same.)
    if (threshold < params.absolute_tolerance) { 
        threshold = params.absolute_tolerance;
    }
    if (std::abs(left - right) > threshold) {
        if (params.report) {
            EXPECT_TRUE(false) << "mismatch in almost-equal floats (expected " << left << ", got " << right << ")";
        }
        return false;
    }

    return true;
}

/**
 * Check if two vectors contain an equal number of almost-equal floats.
 * This compares the corresponding elements from each vector using `compare_almost_equal()`.
 * Any test failure is reported via GoogleTest.
 *
 * @tparam LeftVector_ Some vector-like container of floating-point values.
 * @tparam RightVector_ Another vector-like container of floating-point values.
 * 
 * @param left One of the vectors.
 * @param right The other vector.
 * @param params Further parameters.
 * Note that `CompareAlmostEqualParameters::report` is ignored,
 * any mismatching value will always be reported.
 */
template<class LeftContainer_, class RightContainer_>
void compare_almost_equal_containers(const LeftContainer_& left, const RightContainer_& right, CompareAlmostEqualParameters params) {
    auto n = left.size();
    ASSERT_EQ(n, right.size());

    params.report = false;
    for (decltype(n) i = 0; i < n; ++i) {
        if (!compare_almost_equal(left[i], right[i], params)) {
            EXPECT_TRUE(false) << "mismatch in almost-equal floats at element " << i << " (expected " << left[i] << ", got " << right[i] << ")";
            return;
        }
    }
}

/**
 * @cond
 */
// Back-compatibility.
inline bool compare_almost_equal(double left, double right) {
    return compare_almost_equal(left, right, {});
}

template<class Vector_>
void compare_almost_equal(const Vector_& left, const Vector_& right, double tol = 1e-8) {
    compare_almost_equal_containers(left, right, [&]{
        CompareAlmostEqualParameters params;
        params.relative_tolerance = tol;
        return params;
    }());
}
/**
 * @endcond
 */

}

#endif
