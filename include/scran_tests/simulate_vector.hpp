#ifndef SCRAN_TESTS_SIMULATE_VECTOR_HPP
#define SCRAN_TESTS_SIMULATE_VECTOR_HPP

#include <random>
#include <vector>
#include <cstdint>
#include <type_traits>
#include <cstddef>

/**
 * @file simulate_vector.hpp
 * @brief Simulate a vector of random values.
 */

namespace scran_tests {

/**
 * Type of the random number generator for simulating data.
 */
typedef std::mt19937_64 RngEngine;

/**
 * @tparam Type_ Numeric type of the simulated value.
 * @return -10 if `Type` is signed, otherwise 0.
 */
template<typename Type_>
constexpr Type_ default_simulation_min() {
    if constexpr(std::is_unsigned<Type_>::value) {
        return 0;
    } else {
        return -10;
    }
}

/**
 * @brief Parameters for `simulate_vector()`.
 *
 * @tparam Type_ Numeric type of the simulated value.
 */
template<typename Type_ = double>
struct SimulateVectorParameters {
    /**
     * Density of structural non-zero values.
     */
    double density = 1;

    /**
     * Lower bound for the simulated values.
     * This bound is inclusive.
     */
    Type_ lower = default_simulation_min<Type_>();

    /**
     * Upper bound for the simulated values.
     * This bound is exclusive and should be greater than `SimulateVectorParameters::lower`. 
     */
    Type_ upper = 10;

    /**
     * Seed for the random number generator.
     */
    typename RngEngine::result_type seed = 1234567890;
};

/**
 * @cond
 */
template<typename Type_>
auto create_simulating_distribution(const Type_ lower, const Type_ upper) {
    if constexpr(std::is_floating_point<Type_>::value) {
        return std::uniform_real_distribution<Type_>(lower, upper);
    } else {
        return std::uniform_int_distribution<Type_>(lower, upper - 1);
    }
}

// Back-compatibility.
template<typename Type_>
using SimulationParameters = SimulateVectorParameters<Type_>;
/**
 * @endcond
 */

/**
 * Simulate a vector of random values.
 * If `SimulationParameters::density < 1`, this will be a sparse vector with structural zeros.
 *
 * @tparam Type_ Numeric type of the simulated value.
 *
 * @param length Length of the vector.
 * @param params Simulation parameters.
 *
 * @return Vector of simulated values.
 */
template<typename Type_ = double>
std::vector<Type_> simulate_vector(const typename std::vector<Type_>::size_type length, const SimulateVectorParameters<Type_>& params) {
    RngEngine rng(params.seed);
    auto unif = create_simulating_distribution(params.lower, params.upper);
    std::vector<Type_> values(length);

    if (params.density == 1) {
        for (auto& v : values) {
            v = unif(rng);
        }
    } else {
        std::uniform_real_distribution<double> nonzero(0.0, 1.0);
        for (auto& v : values) {
            if (nonzero(rng) <= params.density) {
                v = unif(rng);
            }
        }
    }

    return values;
}

}

#endif
