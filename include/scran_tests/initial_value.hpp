#ifndef SCRAN_TESTS_INITIAL_VALUE_HPP
#define SCRAN_TESTS_INITIAL_VALUE_HPP

#include <vector>

/**
 * @file initial_value.hpp
 * @brief Choose an initial value for testing zeroing assumptions.
 */

namespace scran_tests {

/**
 * Some functions store their output in a user-supplied pointer to a pre-allocated array.
 * Such functions should not assume that the array has been zeroed.
 * Incorrectly making this assumption can lead to subtle bugs when the functions are called with non-zeroed buffers.
 * To test that this assumption is not present in function `X()`, we can initialize an `output` buffer with a non-zero initial value from the `initial_value()` function below.
 * After calling `X()` with `output`, we can compare the contents of `output` against the expected output to verify that the function ignores the non-zero initial value.
 * If the expected output is not readily available, we could instead initialize a separate vector with another call to `initial_value()` and call `X()` on it.
 * Any inconsistency in the results would indicate that the variable `initial_value()` is affecting the results. 
 * 
 * @return An integer in [1, 255] that is incremented on every call (or rolls back to 1).
 */
inline int initial_value() {
    static int counter = 0;
    if (counter == 255) { // fits in all numeric types.
        counter = 1; 
    } else {
        ++counter;
    }
    return counter;
}

}

#endif
