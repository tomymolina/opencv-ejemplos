cmake_minimum_required(VERSION 2.8.4)
project(opencv_transition)
set(algo 1)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.c )
add_executable(opencv_transition ${SOURCE_FILES})
target_link_libraries( opencv_transition -lm ${OpenCV_LIBS} )
