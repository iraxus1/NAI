cmake_minimum_required(VERSION 3.23)
project(lab10)

set(CMAKE_CXX_STANDARD 14)
#import opencv
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(lab10 main.cpp)
target_link_libraries( lab10 ${OpenCV_LIBS} )