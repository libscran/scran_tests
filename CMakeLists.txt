cmake_minimum_required(VERSION 3.14)

project(scran_test_utils
    VERSION 1.0.0
    DESCRIPTION "Test utilities for libscran"
    LANGUAGES CXX)

add_library(scran_test_utils INTERFACE)
target_compile_features(scran_test_utils INTERFACE cxx_std_17)
target_include_directories(scran_test_utils INTERFACE include)
