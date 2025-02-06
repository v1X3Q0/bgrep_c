cmake_minimum_required(VERSION 3.5)
project(bgrep_c VERSION 0.1.0)

if(NOT DEFINED ANDROID_TOOL_BASE)
    message(WARNING  "need ANDROID_TOOL_BASE")
    set(ANDROID_TOOL_BASE ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(common/local_util)
endif()

set(INCLUDE_LIST ${ANDROID_TOOL_BASE}/common/local_util)

set(LIBDIR_LIST )

set(LINK_LIST bgrep_e localUtil)

add_library(bgrep_e STATIC bgrep_e.cpp)

target_include_directories(bgrep_e PRIVATE ${INCLUDE_LIST})

add_executable(${PROJECT_NAME} main.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_package(getopt_for_windows)
    list(APPEND INCLUDE_LIST ${getopt_for_windows_INCLUDE_DIR})
    list(APPEND LIBDIR_LIST ${getopt_for_windows_LIB_DIR})
    list(APPEND LINK_LIST getopt_for_windows_static)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_LIST})

target_link_directories(${PROJECT_NAME} PRIVATE ${getopt_for_windows_LIB_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIST})

