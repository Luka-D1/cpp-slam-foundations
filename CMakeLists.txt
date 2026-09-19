cmake_minimum_required(VERSION 3.16)  #要求至少使用CMAKE 3.16

project(cpp_slam_foundation LANGUAGES CXX)  #声明项目名称与使用的语言

set(CMAKE_CXX_STANDARD 17)  #使用C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  #如果编译器不使用17，直接报错
set(CMAKE_CXX_EXTENSIONS OFF)   #使用标准C++17，而不是GNU扩展模式。

add_subdirectory(exercises/01_sensor_polymorphism) #继续读取联系目录内的CMAKE文件