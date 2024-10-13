cmake_minimum_required(VERSION 3.10)

# Specify the compiler
set(CMAKE_CXX_COMPILER "clang++-17")

project(MyProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(source/header_files)
include_directories(source/cpp_files)

add_executable(ServerExecutable
    source/cpp_files/server.cpp
    source/cpp_files/network.cpp
    source/header_files/common.hpp
    source/header_files/network.hpp

)

add_executable(ClientExecutable
    source/cpp_files/client.cpp
    source/cpp_files/network.cpp
    source/header_files/common.hpp
    source/header_files/network.hpp
)

