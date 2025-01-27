cmake_minimum_required(VERSION 3.5)

project(CrossCheck CXX)

add_subdirectory(source)
target_include_directories(CrossCheck PUBLIC include)

enable_testing()

add_subdirectory(samples)

target_compile_features(CrossCheck PUBLIC cxx_std_23)
