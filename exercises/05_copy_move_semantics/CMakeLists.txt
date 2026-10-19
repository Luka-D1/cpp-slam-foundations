add_executable(measurement_packet_demo main.cpp)

target_compile_features(measurement_packet_demo PRIVATE cxx_std_17)
target_compile_options(measurement_packet_demo PRIVATE -Wall -Wextra -Wpedantic)

option(LESSON05_DISABLE_OPTIONAL_ELISION
    "Lesson 05: disable optional copy elision for the comparison experiment" OFF)

if(LESSON05_DISABLE_OPTIONAL_ELISION)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(measurement_packet_demo PRIVATE -fno-elide-constructors)
    else()
        message(FATAL_ERROR "Lesson 05 elision comparison requires GCC or Clang")
    endif()
endif()
