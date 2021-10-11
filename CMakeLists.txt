cmake_minimum_required(VERSION 3.18)

project(monitor VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)

add_subdirectory(file_parser)

file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

target_include_directories(monitor PRIVATE 
    "${CURSES_INCLUDE_DIRS}"
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(monitor 
    "${CURSES_LIBRARIES}"
    "FileParser"
    "stdc++fs"
)
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
