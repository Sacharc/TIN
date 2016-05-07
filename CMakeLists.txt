cmake_minimum_required(VERSION 3.2)
project(TIN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(DETECTOR_SOURCE_FILES detector/server.c common/message.h common/messageType.h)
set(MANAGER_SOURCE_FILES manager/client.c common/message.h common/messageType.h)

add_executable(DETECTOR ${DETECTOR_SOURCE_FILES})
add_executable(MANAGER ${MANAGER_SOURCE_FILES})
