cmake_minimum_required(VERSION 3.2)
project(Test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
set(DETECTOR_SOURCE_FILES manager/server.c common/message.h common/messageType.h)
set(MANAGER_SOURCE_FILES detector/client.c common/message.h common/messageType.h)

add_executable(TIN ${SOURCE_FILES})
add_executable(DETECTOR ${DETECTOR_SOURCE_FILES})
add_executable(MANAGER ${MANAGER_SOURCE_FILES})
