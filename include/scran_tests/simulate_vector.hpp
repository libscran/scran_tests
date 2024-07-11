#ifndef SCRAN_TESTS_SIMULATE_VECTOR_HPP
#define SCRAN_TESTS_SIMULATE_VECTOR_HPP

#include <random>
#include <vector>
#include <cstdint>
#include <type_traits>

/**
 * @file simulate_vector.hpp
 * @brief Simulate a vector of random values.
 */

namespace scran_tests {

/**
 * @brief Parameters for `simulate_vector()`.
 *
 * @tparam Type_ Numeric type of the simulated value.
 */
template<typename Type_ = double>
struct SimulationParameters {
    /**
     * @cond
     */
    SimulationParameters() {
        if constexpr(std::is_unsigned<Type_>::value) {
            lower = 0;
        }
    }
    /**
     * @endcond
     */

    /**
     * Density of structural non-zero values.
     */
    double density = 1;

    /**
     * Lower bound for the simulated values.
     * Defaults to 0 if `Type_` is unsigned.
     */
    Type_ lower = -10;

    /**
     * Upper bound for the simulated values.
     */
    Type_ upper = 10;

    /**
     * Seed for the random number generator.
     */
    uint64_t seed = 1234567890;
};

/**
 * Simulate a vector of random values.
 * If `SimulationParameters::density < 1`, this will be a sparse vector with structural zeros.
 *
 * @tparam Type_ Numeric type of the simulated value.
 *
 * @param length Length of the vector.
 * @param params Simulation parameters.
 *
 * @return Vector of values.
 */
template<typename Type_ = double>
std::vector<Type_> simulate_vector(size_t length, const SimulationParameters<Type_>& params) {
    std::mt19937_64 rng(params.seed);

    typename std::conditional<
        std::is_floating_point<Type_>::value, 
        std::uniform_real_distribution<Type_>,
        std::uniform_int_distribution<Type_>
    >::type unif(params.lower, params.upper);

    std::vector<Type_> values(length);

    if (params.density == 1) {
        for (auto& v : values) {
            v = unif(rng);
        }
    } else {
        std::uniform_real_distribution<double> nonzero(0.0, 1.0);
        for (auto& v : values) {
            if (nonzero(rng) < params.density) {
                v = unif(rng);
            }
        }
    }

    return values;
}

}

#endif
