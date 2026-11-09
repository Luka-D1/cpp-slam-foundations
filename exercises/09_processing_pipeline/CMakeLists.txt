add_executable(processing_pipeline_demo main.cpp)

target_compile_features(processing_pipeline_demo PRIVATE cxx_std_17)
target_compile_options(processing_pipeline_demo PRIVATE -Wall -Wextra -Wpedantic)
