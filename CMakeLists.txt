cmake_minimum_required(VERSION 3.14)

project(scran_tests
    VERSION 1.0.0
    DESCRIPTION "Test utilities for libscran"
    LANGUAGES CXX)

add_library(scran_tests INTERFACE)
target_include_directories(scran_tests INTERFACE include)
target_compile_features(scran_tests INTERFACE cxx_std_17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b4aaf97d8f7eaffab79aa15e10a91b331b941fe2.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)
target_link_libraries(scran_tests INTERFACE gtest_main)

# Tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(SCRAN_TEST_UTILS_TESTS "Build scran_tests's test suite." ON)
else()
    option(SCRAN_TEST_UTILS_TESTS "Build scran_tests's test suite." OFF)
endif()

if(SCRAN_TEST_UTILS_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif() 
endif()
