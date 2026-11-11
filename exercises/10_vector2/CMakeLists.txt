add_executable(vector2_demo main.cpp)

target_compile_features(vector2_demo PRIVATE cxx_std_17)
target_compile_options(vector2_demo PRIVATE -Wall -Wextra -Wpedantic)
