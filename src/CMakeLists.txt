cmake_minimum_required(VERSION 3.25.1)

add_subdirectory(scanner)

add_executable(cpplox main.cpp)

target_link_libraries(cpplox PUBLIC scanner)

# Move the executable to the bin directory
set_target_properties(cpplox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)