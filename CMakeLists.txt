cmake_minimum_required(VERSION 3.25.1)
project(cpplox)


message(STATUS "Project Name: " ${PROJECT_NAME})

# Set compiler to clang++
set (CMAKE_CXX_COMPILER, clang++)

# Set output to makefile
set (CMAKE_GENERATOR, "Ninja")

# Set debug flags
set (CMAKE_BUILD_TYPE, Debug)
set (CMAKE_CXX_FLAGS_DEBUG, "-g -O0")
set (CMAKE_CXX_FLAGS_RELEASE, "-O3 -march=native -flto -DNDEBUG")

set (CMAKE_CXX_STANDARD, 14)
set (CMAKE_CXX_STANDARD_REQUIRED, TRUE)
set (CMAKE_EXPORT_COMPILE_COMMANDS, ON)


add_compile_options(-Wall -Wextra -Werror -pedantic)

include_directories("include")

# Add src subdirectory
add_subdirectory(src)

# Move the compile_commands.json file to the root directory
execute_process(COMMAND cp compile_commands.json ${CMAKE_SOURCE_DIR})