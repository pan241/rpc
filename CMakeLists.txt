cmake_minimum_required(VERSION 3.0)
project(rpc)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/src/include/TCP)
include_directories(${PROJECT_SOURCE_DIR}/example)

add_subdirectory(src)
add_subdirectory(example)