//
// Created by jdudziak on 5/28/16.
//

#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include "../logger/LoggerERura.h"

TEST(Test_Logger_1, PositiveNos) {
    bool test[3] = {false};

    LoggerERura loggerERura;
    loggerERura << ERuraLogType::LOGGER_DEBUG;
    loggerERura << "Test abc efg";

    std::ifstream t("eRuraLog.txt");
    std::string test_string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    if(test_string.find("ERura Log History") != std::string::npos) test[0] = true;
    if(test_string.find("[DEBUG]: ")!= std::string::npos) test[1] = true;
    if(test_string.find("Test abc efg")!= std::string::npos) test[2] = true;

    ASSERT_TRUE(test[0]);
    ASSERT_TRUE(test[1]);
    ASSERT_TRUE(test[2]);
}

TEST(Test_Logger_2, PositiveNos) {
    bool test[3] = {false};

    LoggerERura loggerERura;
    loggerERura << ERuraLogType::LOGGER_ERROR;
    loggerERura << ERuraLogType::LOGGER_INFO;
    loggerERura << ERuraLogType::LOGGER_WARNING;

    std::ifstream t("eRuraLog.txt");
    std::string test_string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    if(test_string.find("[INFO]: ")  != std::string::npos) test[0] = true;
    if(test_string.find("[WARNING]: ") != std::string::npos) test[1] = true;
    if(test_string.find("[ERROR]: ") != std::string::npos) test[2] = true;

    ASSERT_TRUE(test[0]);
    ASSERT_TRUE(test[1]);
    ASSERT_TRUE(test[2]);
}
