add_library(
    CrossCheck STATIC
    cross_check.cpp
)

target_compile_features(CrossCheck PUBLIC cxx_std_23)

target_link_options(
    CrossCheck PUBLIC
    $<$<STREQUAL:${CMAKE_CXX_COMPILER_ID},GNU>:
        -lstdc++_libbacktrace -lstdc++exp
    >
)
