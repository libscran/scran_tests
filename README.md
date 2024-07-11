# Test utilities for libscran

![Unit tests](https://github.com/libscran/scran_tests/actions/workflows/run-tests.yaml/badge.svg)
![Documentation](https://github.com/libscran/scran_tests/actions/workflows/doxygenate.yaml/badge.svg)
[![Codecov](https://codecov.io/gh/libscran/scran_tests/graph/badge.svg?token=9OqeoHc56i)](https://codecov.io/gh/libscran/scran_tests)

This repository contains utilities for testing other [**libscran**](https://github.com/libscran) libraries.
To include this in a project, just add the following chunk to the test-specific `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  scran_tests
  GIT_REPOSITORY https://github.com/libscran/scran_tests
  GIT_TAG master # or any version of interest
)

FetchContent_MakeAvailable(scran_tests)

target_link_libraries(mylib INTERFACE scran_tests)
```

This will automatically pull in [GoogleTest](https://github.com/google/googletest) via `FetchContent` so downstream projects don't have to do it themselves.
Then, to use the library, we just have to add the header:

```cpp
#include "scran_tests/scran_tests.hpp"
```

We can now simulate some random data easily: 

```cpp
auto res = scran_tests::simulate_vector(100, scran_tests::SimulationParameters());

auto sparse_res = scran_tests::simulate_vector(100, []{
    scran_tests::SimulationParameters params
    params.density = 0.1;
    return params;
}());

auto int_res = scran_tests::simulate_vector(100, []{
    scran_tests::SimulationParameters<uint16_t> params
    params.density = 0.1;
    return params;
}());
```

Comparison of almost-equal floating-point numbers, given a relative tolerance:

```cpp
scran_tests::compare_almost_equal(1.0, 1.0000000001);

std::vector<double> v1{1.0, 2.000000001};
std::vector<double> v2{1.000000001, 2.0};
scran_tests::compare_almost_equal(v1, v2);
```

Quick construction of vectors for use in `EXPECT_EQ()`:

```cpp
std::vector<double> big_array(1000);
auto subset = scran_tests::vector_n(big_array.data() + 100, 500); // slice from [100, 600)
```

Expect specific error messages:

```cpp
expect_error([&]() { throw std::runtime_error("foobar"); }, "foo");
```

Check out the [documentation](https://libscran.github.io/scran_tests) for more details.
