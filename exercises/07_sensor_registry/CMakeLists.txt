add_executable(sensor_registry_demo main.cpp)

target_compile_features(sensor_registry_demo PRIVATE cxx_std_17)
target_compile_options(sensor_registry_demo PRIVATE -Wall -Wextra -Wpedantic)
