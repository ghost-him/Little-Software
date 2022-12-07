cmake_minimum_required(VERSION 3.22)
project(PROJECT
        VERSION 0.1.0
        )

# 选择c++的版本
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Debug")

# 添加头文件
include_directories(include)
# 源文件
aux_source_directory(resource resources)
# 头文件
aux_source_directory(include includes)

# 设置程序的名字
add_executable(PROJECT ${resources} ${includes})

# 添加连接库
#　find_library(lib_pthread pthread REQUIRED)
#　target_link_libraries(PROJECT ${lib_pthread})

