cmake_minimum_required(VERSION 3.5)

set(VERBOSE)

project(CrossCheck CXX)

add_subdirectory(source)
target_include_directories(CrossCheck PUBLIC include)

enable_testing()

add_subdirectory(samples)
