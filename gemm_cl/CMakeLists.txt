cmake_minimum_required(VERSION 3.10)
project(gemm)

include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(../include)

aux_source_directory(. GEMM_SRC)

file(COPY ./gemm.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${GEMM_SRC})
target_link_libraries(${PROJECT_NAME} cltk ${OpenCL_LIBRARIES} "m")
