#include "scran_tests/compare_almost_equal.hpp"

TEST(CompareAlmostEqual, Scalar) {
    EXPECT_TRUE (scran_tests::compare_almost_equal(1.0, 1.0));
    EXPECT_TRUE (scran_tests::compare_almost_equal(1.0, 1.000000005));
    EXPECT_FALSE(scran_tests::compare_almost_equal(1.0, 1.00000005, /* tol */ 1e-8, /* report = */ false));

    EXPECT_TRUE (scran_tests::compare_almost_equal(10.0, 10.0));
    EXPECT_TRUE (scran_tests::compare_almost_equal(10.0, 10.00000005));
    EXPECT_FALSE(scran_tests::compare_almost_equal(10.0, 10.0000005, /* tol */ 1e-8, /* report = */ false));

    EXPECT_TRUE (scran_tests::compare_almost_equal(1e-15, 1e-16));
    EXPECT_FALSE(scran_tests::compare_almost_equal(1e-15, 1e-8, /* tol */ 1e-8, /* report = */ false));
}

TEST(CompareAlmostEqual, Vector) {
    std::vector<double> alpha{ 1, 10.00000005, 100 };
    std::vector<double> bravo{ 1.000000005, 10, 100.0000005 };
    scran_tests::compare_almost_equal(alpha, bravo);
}
