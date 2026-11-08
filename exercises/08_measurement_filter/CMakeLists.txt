add_executable(measurement_filter_demo main.cpp)

target_compile_features(measurement_filter_demo PRIVATE cxx_std_17)
target_compile_options(measurement_filter_demo PRIVATE -Wall -Wextra -Wpedantic)
