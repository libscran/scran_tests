#ifndef SCRAN_TESTS_SIMULATE_COMPRESSED_SPARSE_MATRIX_HPP
#define SCRAN_TESTS_SIMULATE_COMPRESSED_SPARSE_MATRIX_HPP

#include <random>
#include <vector>
#include <type_traits>
#include <cstddef>

#include "simulate_vector.hpp"

/**
 * @file simulate_compressed_sparse_matrix.hpp
 * @brief Simulate a compressed sparse matrix.
 */

namespace scran_tests {

/**
 * @brief Parameters for `simulate_compressed_sparse_matrix()`.
 *
 * @tparam Data_ Numeric type of the simulated value.
 */
template<typename Data_ = double>
struct SimulateCompressedSparseMatrixParameters {
    /**
     * Density of structural non-zero values.
     */
    double density = 0.2;

    /**
     * Lower bound for the simulated values.
     * This bound is inclusive.
     */
    Data_ lower = default_simulation_min<Data_>(); 

    /**
     * Upper bound for the simulated values.
     * This bound is exclusive.
     */
    Data_ upper = 10;

    /**
     * Seed for the random number generator.
     */
    typename RngEngine::result_type seed = 1234567890;
};

/**
 * @brief Results of `simulate_compressed_sparse_matrix()`.
 * @tparam Data_ Numeric type of the data.
 * @tparam Index_ Integer type of the indices.
 * @tparam Pointer_ Integer type of the compressed sparse pointers.
 */
template<typename Data_, typename Index_, typename Pointer_>
struct SimulatedCompressedSparseMatrix {
    /**
     * Extent of the primary dimension.
     */ 
    Index_ primary;

    /**
     * Extent of the secondary dimension.
     */ 
    Index_ secondary;

    /**
     * Values of the structural non-zero elements.
     */
    std::vector<Data_> data;

    /**
     * Indices of the structural non-zero elements, along the secondary dimension.
     */
    std::vector<Index_> index;

    /**
     * Pointers specifying the first and last non-zero element for each primary dimension element.
     */
    std::vector<Pointer_> pointers;
};

/**
 * @tparam Data_ Numeric type of the data.
 * @tparam Index_ Integer type of the indices.
 * @tparam Pointer_ Integer type of the compressed sparse pointers.
 *
 * @param primary Extent of the primary dimension, i.e., along which the structural non-zero elements are compressed.
 * For example, in a compressed sparse row matrix, the rows would be the primary dimension.
 * @param secondary Extent of the secondary dimension, i.e., not the primary dimension.
 * @param params Simulation parameters.
 *
 * @return Contents of a simulated compressed sparse matrix.
 */
template<typename Data_ = double, typename Index_ = int, typename Pointer_ = std::size_t>
SimulatedCompressedSparseMatrix<Data_, Index_, Pointer_> simulate_compressed_sparse_matrix(
    Index_ primary,
    Index_ secondary,
    const SimulateCompressedSparseMatrixParameters<Data_>& params
) {
    RngEngine rng(params.seed);
    auto unif = create_simulating_distribution(params.lower, params.upper);

    SimulatedCompressedSparseMatrix<Data_, Index_, Pointer_> output;
    output.primary = primary;
    output.secondary = secondary;
    output.pointers.reserve(static_cast<typename std::vector<Pointer_>::size_type>(primary) + 1);
    output.pointers.push_back(0);

    std::uniform_real_distribution<double> nonzero(0.0, 1.0);
    for (Index_ p = 0; p < primary; ++p) {
        for (Index_ s = 0; s < secondary; ++s) {
            if (nonzero(rng) <= params.density) {
                output.index.push_back(s);
                output.data.push_back(unif(rng));
            }
        }
        output.pointers.push_back(output.index.size());
    }

    return output;
}

}

#endif
