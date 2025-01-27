add_executable(
    samples main.cpp
)

target_link_libraries(samples PUBLIC CrossCheck)

enable_testing()

add_test(NAME run_sample COMMAND samples)
