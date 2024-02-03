cmake_minimum_required(VERSION 3.27)
project(parser_operator_version)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(parser_operator_version
        main.cpp
        main.hpp
        type.cpp
        type.cpp
)
