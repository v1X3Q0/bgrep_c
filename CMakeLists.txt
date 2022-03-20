cmake_minimum_required(VERSION 3.0.0)
project(BGREP_C VERSION 0.1.0)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(FATAL_ERROR "need ANDROID_TOOL_BASE")
endif()

add_library(bgrep_e STATIC bgrep_e.cpp)
target_include_directories(bgrep_e PRIVATE ${ANDROID_TOOL_BASE}/common/local_util)

add_executable(bgrep_c main.cpp)
target_include_directories(bgrep_c PRIVATE ${ANDROID_TOOL_BASE}/common/local_util)
target_link_libraries(bgrep_c PRIVATE bgrep_e localUtil)
