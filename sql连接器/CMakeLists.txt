cmake_minimum_required(VERSION 3.23)
project(test)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

aux_source_directory(include includes)
aux_source_directory(resource resources)

add_executable(test ${includes} ${resources})

find_library(lib_mysql mariadb REQUIRED)
target_link_libraries(test ${lib_mysql})
target_link_libraries(test ${lib_mysql})
