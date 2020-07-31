cmake_minimum_required(VERSION 3.10)
project(example_image)

include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(../include)
include_directories(../stb)

aux_source_directory(. EXAMPLE_IMAGE_SRC)

file(COPY ./image_test.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./PSV.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${EXAMPLE_IMAGE_SRC})
target_link_libraries(${PROJECT_NAME} cltk ${OpenCL_LIBRARY} "m")
