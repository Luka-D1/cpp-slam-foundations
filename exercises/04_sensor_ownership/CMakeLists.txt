add_executable(sensor_ownership_demo main.cpp)

target_compile_features(sensor_ownership_demo PRIVATE cxx_std_17)

target_compile_options(sensor_ownership_demo PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
