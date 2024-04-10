cmake_minimum_required(VERSION 3.25.1)
project(cpplox-ast)

message(STATUS "Project Part: " ${PROJECT_NAME})

aux_source_directory(. DIR_SRCS)

add_library(ast OBJECT ${DIR_SRCS})