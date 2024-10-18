project(boundaryRecognition LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

set(Boost_USE_STATIC_LIBS ON)

find_package(OpenCV CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

add_library(openCVlibs INTERFACE)
include_directories(src)
include_directories(include)
include_directories(${Boost_INCUDE_DIR})

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

add_executable(BRCli src/main.cpp src/boundaryRecognition.cpp src/server.cpp include/boundaryRecognition.h include/server.h)
target_link_libraries(BRCli PRIVATE openCVlibs ${Boost_LIBRARIES})