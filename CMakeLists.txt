if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Not support build in source dir")
endif()

cmake_minimum_required(VERSION 3.20)

project(my-allocator LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
file(GLOB SRC "*.cpp")
message(STATUS "find files ${SRC}")

add_executable(allocator ${SRC})

target_include_directories(allocator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)