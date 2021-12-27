cmake_minimum_required(VERSION 3.0.0)
project(BGREP_C VERSION 0.1.0)

add_library(bgrep_e STATIC bgrep_e.cpp)
add_executable(bgrep_c main.cpp)
target_link_libraries(bgrep_c PRIVATE bgrep_e)