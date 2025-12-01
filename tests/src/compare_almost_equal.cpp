#include <limits>
#include <vector>

#include "scran_tests/compare_almost_equal.hpp"

TEST(CompareAlmostEqual, Scalar) {
    scran_tests::CompareAlmostEqualParameters params;
    params.relative_tolerance = 1e-8;
    params.report = false;

    EXPECT_TRUE (scran_tests::compare_almost_equal(1.0, 1.0, params));
    EXPECT_TRUE (scran_tests::compare_almost_equal(1.0, 1.000000005, params));
    EXPECT_FALSE(scran_tests::compare_almost_equal(1.0, 1.00000005, params));

    EXPECT_TRUE (scran_tests::compare_almost_equal(10.0, 10.0, params));
    EXPECT_TRUE (scran_tests::compare_almost_equal(10.0, 10.00000005, params));
    EXPECT_FALSE(scran_tests::compare_almost_equal(10.0, 10.0000005, params));

    EXPECT_TRUE (scran_tests::compare_almost_equal(1e-15, 1e-16, params));
    EXPECT_FALSE(scran_tests::compare_almost_equal(1e-15, 1e-8, params));

    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    EXPECT_TRUE (scran_tests::compare_almost_equal(nan, nan, params));
    EXPECT_FALSE(scran_tests::compare_almost_equal(nan, 2.0, params));
    EXPECT_FALSE(scran_tests::compare_almost_equal(nan, nan, [](){
        scran_tests::CompareAlmostEqualParameters params;
        params.nan_equal = false;
        params.report = false;
        return params;
    }()));
}

TEST(CompareAlmostEqual, Vector) {
    std::vector<double> alpha{ 1, 10.00000005, 100 };
    std::vector<double> bravo{ 1.000000005, 10, 100.0000005 };
    scran_tests::compare_almost_equal_containers(alpha, bravo, scran_tests::CompareAlmostEqualParameters());
}
