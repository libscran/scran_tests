#ifndef SCRAN_TEST_SIMULATE_VECTOR
#define SCRAN_TEST_SIMULATE_VECTOR

#include <random>
#include <vector>
#include <cstdint>

template<typename Float_ = double>
std::vector<Float_> simulate_vector(size_t length, double lower = -10, double upper = 10, uint64_t seed = 1234567890) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<Float_> unif(lower, upper);
    std::vector<Float_> values(length);
    for (auto& v : values) {
        v = unif(rng);
    }
    return values;
}

template<typename Float_ = double>
std::vector<Float_> simulate_sparse_vector(size_t length, double density, double lower = -10, double upper = 10, uint64_t seed = 1234567890) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<Float_> nonzero(0.0, 1.0);
    std::uniform_real_distribution<Float_> unif(lower, upper);
    std::vector<Float_> values(length);
    for (auto& v : values) {
        if (nonzero(rng) < density) {
            v = unif(rng);
        }
    }
    return values;
}

#endif
