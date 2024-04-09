cmake_minimum_required(VERSION 3.25.1)
project(cpplox-scanner)

message(STATUS "Project Part: " ${PROJECT_NAME})

aux_source_directory(. DIR_SRCS)

add_library(scanner OBJECT ${DIR_SRCS})

# set_target_properties(scanner PROPERTIES
#     LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output
# )

# target_include_directories(scanner PRIVATE ${CMAKE_SOURCE_DIR}/include)