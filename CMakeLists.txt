#W celu zbudowania plikow testowych odkomentowac wykomentowane linie oraz linie numer 9

cmake_minimum_required(VERSION 2.8)
project(TIN)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Project_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pthread")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pthread -lgtest")

set(DETECTOR_SOURCE_FILES
        detector/server.cpp
        common/message.h
        common/messageType.h
        common/messageType.cpp
        common/Constants.h)

set(MANAGER_SOURCE_FILES
        manager/client.cc
        common/Constants.h
        common/message.h
        common/messageType.h
        common/messageType.cpp
        manager/DetectorHistory.cpp
        manager/DetectorHistory.h
        manager/HistoryRecord.cpp
        manager/HistoryRecord.h
        logger/ERuraLogType.h
        logger/LoggerERura.cpp
        logger/LoggerERura.h
        manager/MessageHandler.cpp
        manager/MessageHandler.h
        businessLogic/Statistics.cpp
        businessLogic/Statistics.h
        http_module/HttpHandler.cpp
        http_module/HttpHandler.h
        manager/cli/CommandLineInterface.cpp
        manager/cli/CommandLineInterface.h)

add_executable(DETECTOR ${DETECTOR_SOURCE_FILES})
add_executable(MANAGER ${MANAGER_SOURCE_FILES})

#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})
#
#set(TEST_FILES tests/BusinnesLogicTests.cpp
#        common/message.h
#        common/messageType.h
#        manager/DetectorHistory.cpp
#        manager/DetectorHistory.h
#        manager/HistoryRecord.cpp
#        manager/HistoryRecord.h
#        logger/ERuraLogType.h
#        logger/LoggerERura.cpp
#        logger/LoggerERura.h
#        manager/MessageHandler.cpp
#        manager/MessageHandler.h
#        businessLogic/Statistics.cpp
#        businessLogic/Statistics.h
#        http_module/HttpHandler.cpp
#        http_module/HttpHandler.h)
#
#add_executable(runTests
#        ${TEST_FILES}
#        )
#
#target_link_libraries(runTests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES}  pthread)