add_executable(imu_buffer_demo main.cpp)

target_compile_features(imu_buffer_demo PRIVATE cxx_std_17)

target_compile_options(imu_buffer_demo PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
