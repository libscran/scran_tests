add_executable(
    libtest 
    src/compare_almost_equal.cpp
    src/vector_n.cpp
    src/simulate_vector.cpp
    src/simulate_compressed_sparse_matrix.cpp
    src/expect_error.cpp
    src/initial_value.cpp
)

target_link_libraries(
    libtest
    scran_tests
)

target_compile_options(libtest PRIVATE -Wall -Werror -Wpedantic -Wextra)

option(CODE_COVERAGE "Enable coverage testing" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(libtest PRIVATE -O0 -g --coverage)
    target_link_options(libtest PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(libtest)
