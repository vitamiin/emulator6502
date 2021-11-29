cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(emulator VERSION 1.0)

file(GLOB_RECURSE SRC_FILES src/*.cc )

add_executable(emulator ${SRC_FILES})
target_include_directories(emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
