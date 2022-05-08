cmake_minimum_required(VERSION 3.16.5)
project(main)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp config.h config.cpp anchor_generator.h anchor_generator.cpp ulsMatF.h 3rdparty/tvm/apps/howto_deploy/tvm_runtime_pack.cc)

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