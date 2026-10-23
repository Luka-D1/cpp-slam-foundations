add_executable(imu_sequence_demo main.cpp)

target_compile_features(imu_sequence_demo PRIVATE cxx_std_17)
target_compile_options(imu_sequence_demo PRIVATE -Wall -Wextra -Wpedantic)
