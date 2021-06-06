cmake_minimum_required(VERSION 3.10)
project(InOneWeekend)

include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(../../include)
include_directories(..)

set(BMP_SRC bmp.cpp)
set(VEC3_SRC vec3.cpp)
set(RAY_SRC ray.cpp)
set(REDSPHERE_SRC redsphere.cpp)
set(NORMALSPHERE_SRC normalsphere.cpp)

file(COPY bmp.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY vec3.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ray.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY redsphere.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY normalsphere.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(bmp ${BMP_SRC})
target_link_libraries(bmp cltk ${OpenCL_LIBRARIES}  "m")

add_executable(vec3 ${VEC3_SRC})
target_link_libraries(vec3 cltk ${OpenCL_LIBRARIES}  "m")

add_executable(ray ${RAY_SRC})
target_link_libraries(ray cltk ${OpenCL_LIBRARIES}  "m")

add_executable(redsphere ${REDSPHERE_SRC})
target_link_libraries(redsphere cltk ${OpenCL_LIBRARIES}  "m")

add_executable(normalsphere ${NORMALSPHERE_SRC})
target_link_libraries(normalsphere cltk ${OpenCL_LIBRARIES}  "m")
