cmake_minimum_required(VERSION 3.10)
project(cltk)

find_package(OpenCL REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")

include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(include)

aux_source_directory(src CLTK_SRC)

add_library(${PROJECT_NAME} SHARED ${CLTK_SRC})
target_link_libraries(${PROJECT_NAME} ${OPENCL_LIBRARIES})

add_subdirectory(example)
add_subdirectory(example_image)
add_subdirectory(gemm_cl)
