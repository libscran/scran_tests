cmake_minimum_required(VERSION 3.24)

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
  URL https://github.com/google/googletest/archive/d144031940543e15423a25ae5a8a74141044862f.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)
target_link_libraries(scran_tests INTERFACE gtest_main)

# Tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(SCRAN_TESTS_TESTS "Build scran_tests's test suite." ON)
else()
    option(SCRAN_TESTS_TESTS "Build scran_tests's test suite." OFF)
endif()

if(SCRAN_TESTS_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif() 
endif()
