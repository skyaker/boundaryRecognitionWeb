add_executable(BRCli main.cpp boundaryRecognition.h 
                             boundaryRecognition.cpp)
target_link_libraries(BRCli PRIVATE openCVlibs)