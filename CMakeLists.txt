cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(gctest CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(gc)

add_executable(gctest main.cpp)
target_link_libraries(gctest gc-lib)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT gctest)