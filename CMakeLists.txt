cmake_minimum_required(VERSION 3.27)
project(PasswordFilter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_SYSTEM_VERSION 10.0)

add_library(PasswordFilter SHARED library.cpp)
target_link_libraries(PasswordFilter ntdll)
