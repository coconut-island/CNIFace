cmake_minimum_required(VERSION 3.16.5)
project(main)
set(CMAKE_CXX_STANDARD 14)

file(GLOB RETINA_FACE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/retinaface/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/retinaface/*.cpp
)
file(GLOB MOBILE_FACE_NET_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/mobilefacenet/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mobilefacenet/*.cpp
        )

set(SOURCE_FILES
        main.cpp
        common.h
        common.cpp
        3rdparty/tvm/apps/howto_deploy/tvm_runtime_pack.cc
        ${RETINA_FACE_SRC}
        ${MOBILE_FACE_NET_SRC}
)

find_package(OpenCV REQUIRED)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        "3rdparty/tvm/include"
        "3rdparty/tvm/3rdparty/dmlc-core/include"
        "3rdparty/tvm/3rdparty/dlpack/include"
)
link_directories(
        ${OpenCV_LIBRARY_DIRS}
        "/usr/local/lib"
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} dl pthread)