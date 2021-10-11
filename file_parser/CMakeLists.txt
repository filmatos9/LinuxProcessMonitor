cmake_minimum_required(VERSION 3.18)

# set the project name
project(FileParser VERSION 0.1)

# add the executable
add_library(FileParser "src/file_parser.cpp")

# set the includes
target_include_directories(FileParser PUBLIC "$(PROJECT_SRC_DIR)/include")