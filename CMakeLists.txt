cmake_minimum_required(VERSION 2.8)
project(CarNum)
set(CMAKE_CXX_STANDARD 14)


set(CarNumOcr_LIBS CarNumOcr)
set(CarNumOcr_INCLUDE_DIRS ${CMAKE_SOURCE_DIR})

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})

include_directories(.
        include
        include/src
        )


set(SRC_DETECTION include/src/PlateDetection.cpp  include/src/util.h)
set(SRC_FINEMAPPING include/src/FineMapping.cpp )
set(SRC_FASTDESKEW include/src/FastDeskew.cpp )
set(SRC_SEGMENTATION include/src/PlateSegmentation.cpp )
set(SRC_RECOGNIZE include/src/Recognizer.cpp include/src/CNNRecognizer.cpp)
set(SRC_PIPLINE include/src/Pipeline.cpp)
set(SRC_SEGMENTATIONFREE include/src/SegmentationFreeRecognizer.cpp)


add_library(
        CarNumOcr
        STATIC
        CarNumOcr.cpp
        ${SRC_DETECTION}
        ${SRC_FINEMAPPING}
        ${SRC_SEGMENTATIONFREE}
        ${SRC_SEGMENTATION}
        ${SRC_PIPLINE}
        ${SRC_FASTDESKEW}
        ${SRC_RECOGNIZE}
)
add_executable(
        CarNum
        main.cpp
)

target_link_libraries(CarNumOcr ${OpenCV_LIBS})
target_link_libraries(CarNum CarNumOcr)
