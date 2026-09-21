add_executable(reference_pointer_demo main.cpp)

target_compile_options(
    reference_pointer_demo
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
