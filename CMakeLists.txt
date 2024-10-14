cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "clang++-17")

project(MyProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(source/header_files)

set(SERVER_SOURCES
    source/cpp_files/server.cpp
)

set(CLIENT_SOURCES
    source/cpp_files/client.cpp
)

add_executable(Server ${SERVER_SOURCES})
add_executable(Client ${CLIENT_SOURCES})

# Link with pthread
target_link_libraries(Server PRIVATE pthread)
target_link_libraries(Client PRIVATE pthread)

