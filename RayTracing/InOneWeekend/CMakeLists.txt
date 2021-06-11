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
set(HITTABLE_LIST_SRC hittable_list.cpp)
set(ANTI_ALIASING_SRC anti_aliasing.cpp)
set(DIFFUSE_SRC diffuse.cpp)
set(MATERIAL_SRC material.cpp)
set(DIELECTRIC_SRC dielectric.cpp)
set(CAMERA_SRC camera.cpp)
set(CAMERA2_SRC camera2.cpp)
set(FINAL_SRC final.cpp)

file(COPY bmp.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY vec3.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ray.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY redsphere.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY normalsphere.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY hittable_list.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY anti_aliasing.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY diffuse.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY material.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY material.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY dielectric.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY camera.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY camera2.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY final.cl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(bmp ${BMP_SRC})
target_link_libraries(bmp cltk ${OpenCL_LIBRARIES} "m")

add_executable(vec3 ${VEC3_SRC})
target_link_libraries(vec3 cltk ${OpenCL_LIBRARIES} "m")

add_executable(ray ${RAY_SRC})
target_link_libraries(ray cltk ${OpenCL_LIBRARIES} "m")

add_executable(redsphere ${REDSPHERE_SRC})
target_link_libraries(redsphere cltk ${OpenCL_LIBRARIES} "m")

add_executable(normalsphere ${NORMALSPHERE_SRC})
target_link_libraries(normalsphere cltk ${OpenCL_LIBRARIES} "m")

add_executable(hittable_list ${HITTABLE_LIST_SRC})
target_link_libraries(hittable_list cltk ${OpenCL_LIBRARIES} "m")

add_executable(anti_aliasing ${ANTI_ALIASING_SRC})
target_link_libraries(anti_aliasing cltk ${OpenCL_LIBRARIES} "m")

add_executable(diffuse ${DIFFUSE_SRC})
target_link_libraries(diffuse cltk ${OpenCL_LIBRARIES} "m")

add_executable(material ${MATERIAL_SRC})
target_link_libraries(material cltk ${OpenCL_LIBRARIES} "m")

add_executable(dielectric ${DIELECTRIC_SRC})
target_link_libraries(dielectric cltk ${OpenCL_LIBRARIES} "m")

add_executable(camera ${CAMERA_SRC})
target_link_libraries(camera cltk ${OpenCL_LIBRARIES} "m")

add_executable(camera2 ${CAMERA2_SRC})
target_link_libraries(camera2 cltk ${OpenCL_LIBRARIES} "m")

add_executable(final ${FINAL_SRC})
target_link_libraries(final cltk ${OpenCL_LIBRARIES} "m")
