cmake_minimum_required( VERSION 3.27.7 )

project(boundaryRecognition LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

find_package(OpenCV CONFIG REQUIRED)

add_subdirectory(src)

add_library(openCVlibs INTERFACE)

function(libraries_add_tag openCVlibs)
target_link_libraries(openCVlibs INTERFACE ${ARGN})
endfunction()

libraries_add_tag(openCVlibs opencv_core
    opencv_highgui
    opencv_imgproc
    opencv_imgcodecs
    opencv_videoio
    opencv_features2d
    opencv_calib3d
    opencv_objdetect
    opencv_flann
)