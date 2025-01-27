add_executable(
    samples main.cpp
)

target_link_libraries(samples PUBLIC CrossCheck)

add_test(NAME run_sample COMMAND samples)
