cmake_minimum_required(VERSION 2.8.4)
project(practica_6)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.c )
add_executable(practica_6 ${SOURCE_FILES})
target_link_libraries( practica_6 -lm ${OpenCV_LIBS} )
