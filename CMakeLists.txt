cmake_minimum_required(VERSION 2.8)
project(TIN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(DETECTOR_SOURCE_FILES detector/server.cpp common/message.h common/messageType.h logger/ERuraLogType.h logger/LoggerERura.cpp logger/LoggerERura.cpp  businessLogic/Statistics.cpp businessLogic/Statistics.h)
set(MANAGER_SOURCE_FILES manager/client.cc common/message.h common/messageType.h manager/DetectorHistory.cpp manager/DetectorHistory.h manager/HistoryRecord.cpp manager/HistoryRecord.h)

add_executable(DETECTOR ${DETECTOR_SOURCE_FILES})
add_executable(MANAGER ${MANAGER_SOURCE_FILES})
