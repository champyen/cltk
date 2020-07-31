cmake_minimum_required(VERSION 3.10)
project(example)

include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(../include)

aux_source_directory(. EXAMPLE_SRC)

file(COPY ./test.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${EXAMPLE_SRC})
target_link_libraries(${PROJECT_NAME} ${OpenCL_LIBRARIES} cltk "m")
