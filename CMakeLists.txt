cmake_minimum_required(VERSION 3.16.3)
project(cniface)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

option(SAMPLES "compile samples" ON)

message("SAMPLES=${SAMPLES}")

set(SYSTEM_INCLUDE "/usr/local/include")
set(SYSTEM_LIB "/usr/local/lib")

file(GLOB COMMON_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp
        )

file(GLOB RETINA_FACE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/retinaface/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/retinaface/*.cpp
        )

file(GLOB ARC_FACE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/arcface/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/arcface/*.cpp
        )

file(GLOB UTILS_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp
        )

file(GLOB GRPC_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/grpc/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/grpc/*.cc
        )

file(GLOB SERVICE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/service/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/service/*.cpp
        )

set(SOURCE_FILES
        main.cpp
        ${COMMON_SRC}
        ${UTILS_SRC}
        ${RETINA_FACE_SRC}
        ${ARC_FACE_SRC}
        ${COMMON_SRC}
        ${UTILS_SRC}
        ${GRPC_SRC}
        ${SERVICE_SRC}
)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${SYSTEM_INCLUDE}
)

link_directories(
        ${OpenCV_LIBRARY_DIRS}
        ${SYSTEM_LIB}
)

# main
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} dl pthread tvm_runtime gflags grpc++ protobuf absl_synchronization)

# samples
if(SAMPLES)
    add_executable (detect ./samples/detect.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC})
    target_link_libraries(detect ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (detect_camera ./samples/detect_camera.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC})
    target_link_libraries(detect_camera ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (recognition ./samples/recognition.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC} ${ARC_FACE_SRC})
    target_link_libraries(recognition ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (recognition_camera ./samples/recognition_camera.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC} ${ARC_FACE_SRC})
    target_link_libraries(recognition_camera ${OpenCV_LIBS} dl pthread tvm_runtime)
endif()


install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)