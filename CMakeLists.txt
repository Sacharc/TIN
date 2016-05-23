cmake_minimum_required(VERSION 3.0.2)
project(TIN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

<<<<<<< HEAD
set(DETECTOR_SOURCE_FILES detector/server.cpp common/message.h common/messageType.h logger/ERuraLogType.h logger/LoggerERura.cpp logger/LoggerERura.cpp  businessLogic/Statistics.cpp businessLogic/Statistics.h)
set(MANAGER_SOURCE_FILES manager/client.cc common/message.h common/messageType.h manager/DetectorHistory.cpp manager/DetectorHistory.h manager/HistoryRecord.cpp manager/HistoryRecord.h)
=======
set(DETECTOR_SOURCE_FILES detector/server.cpp common/message.h common/messageType.h)
set(MANAGER_SOURCE_FILES manager/client.cc common/message.h common/messageType.h manager/DetectorHistory.cpp manager/DetectorHistory.h manager/HistoryRecord.cpp manager/HistoryRecord.h
        logger/ERuraLogType.h logger/LoggerERura.cpp logger/LoggerERura.h manager/MessageHandler.cpp manager/MessageHandler.h )
>>>>>>> origin/master

add_executable(DETECTOR ${DETECTOR_SOURCE_FILES})
add_executable(MANAGER ${MANAGER_SOURCE_FILES})
