add_executable(sensor_demo main.cpp)  #用main.cpp构建名为sensor_demo的可执行程序

target_compile_options(     #只为该目标打开编译警告
    sensor_demo
    PRIVATE                 #这些选项只属于sensor_demo
        -Wall
        -Wextra
        -Wpedantic
)