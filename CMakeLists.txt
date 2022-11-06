cmake_minimum_required(VERSION 3.16.3)
project(cniface)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

option(BUILD_SAMPLES "BUILD SAMPLES" ON)

message("BUILD_SAMPLES=${BUILD_SAMPLES}")

set(SYSTEM_INCLUDE "/usr/local/include")
set(SYSTEM_LIB "/usr/local/lib")

set(GAMMA_INCLUDE /usr/local/include/gamma/ /usr/local/include/gamma/third_party)

file(GLOB COMMON_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp
        )

file(GLOB RETINA_FACE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/retinaface/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/retinaface/*.cpp
        )

file(GLOB MNET_COV2_FACE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/mnet_cov2/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/mnet_cov2/*.cpp
        )

file(GLOB ARC_FACE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/arcface/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/arcface/*.cpp
        )

file(GLOB GENDER_AGE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/genderage/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/genderage/*.cpp
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

file(GLOB REPOSITORY_SERVICE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/repository/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/repository/*.cpp
        )

file(GLOB PADDLE_OCR_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/ppocr/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ppocr/*.cpp
        )

set(SOURCE_FILES
        main.cpp
        ${COMMON_SRC}
        ${UTILS_SRC}
        ${RETINA_FACE_SRC}
        ${MNET_COV2_FACE_SRC}
        ${ARC_FACE_SRC}
        ${GENDER_AGE_SRC}
        ${COMMON_SRC}
        ${UTILS_SRC}
        ${GRPC_SRC}
        ${SERVICE_SRC}
        ${REPOSITORY_SERVICE_SRC}
        ${PADDLE_OCR_SRC}
)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${SYSTEM_INCLUDE}
        ${GAMMA_INCLUDE}
)

link_directories(
        ${OpenCV_LIBRARY_DIRS}
        ${SYSTEM_LIB}
        ${GAMMA_LIB}
)

# main
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} dl pthread tvm_runtime gflags grpc++ protobuf absl_synchronization gamma)

# samples
if(BUILD_SAMPLES)
    add_executable (detect ./samples/detect.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC})
    target_link_libraries(detect ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (detect_camera ./samples/detect_camera.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC})
    target_link_libraries(detect_camera ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (recognition ./samples/recognition.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC} ${ARC_FACE_SRC})
    target_link_libraries(recognition ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (recognition_camera ./samples/recognition_camera.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC} ${ARC_FACE_SRC})
    target_link_libraries(recognition_camera ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (detect_cov2 ./samples/detect_cov2.cpp ${COMMON_SRC} ${UTILS_SRC} ${MNET_COV2_FACE_SRC})
    target_link_libraries(detect_cov2 ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (detect_cov2_camera ./samples/detect_cov2_camera.cpp ${COMMON_SRC} ${UTILS_SRC} ${MNET_COV2_FACE_SRC})
    target_link_libraries(detect_cov2_camera ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (attribute ./samples/attribute.cpp ${COMMON_SRC} ${UTILS_SRC} ${RETINA_FACE_SRC} ${GENDER_AGE_SRC})
    target_link_libraries(attribute ${OpenCV_LIBS} dl pthread tvm_runtime)

    add_executable (repository ./samples/repository.cpp ${COMMON_SRC} ${UTILS_SRC} ${ARC_FACE_SRC} ${RETINA_FACE_SRC} ${GENDER_AGE_SRC} ${REPOSITORY_SERVICE_SRC})
    target_link_libraries(repository ${OpenCV_LIBS} dl pthread tvm_runtime gamma)

    add_executable (ppocr ./samples/ppocr.cpp ${COMMON_SRC} ${UTILS_SRC} ${PADDLE_OCR_SRC})
    target_link_libraries(ppocr ${OpenCV_LIBS} dl pthread tvm_runtime)
endif()


install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
install(DIRECTORY resources DESTINATION /cniface)