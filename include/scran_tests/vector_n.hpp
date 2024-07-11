#ifndef SCRAN_TESTS_VECTOR_N_HPP
#define SCRAN_TESTS_VECTOR_N_HPP

#include <vector>

/**
 * @file vector_n.hpp
 * @brief Quickly create a vector for `EXPECT_EQ`.
 */

namespace scran_tests {

/**
 * Quickly create a vector from an array, primarily for convenient use in `EXPECT_EQ` with another vector.
 *
 * @param[in] ptr Pointer to the start of an array.
 * @param n Length of the array.
 *
 * @return Vector of the array's values.
 */
template<typename Type_>
std::vector<Type_> vector_n(const Type_* ptr, size_t n) {
    return std::vector<Type_>(ptr, ptr + n);
}

}

#endif
