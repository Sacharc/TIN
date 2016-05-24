//
// Created by jdudziak on 5/22/16.
//
// Class realize logger to file in all functions of program

#ifndef TIN_LOGGERERURA_H
#define TIN_LOGGERERURA_H

#include "../logger/ERuraLogType.h"
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

///using namespace std;

class LoggerERura {

    public:
        std::ofstream logFile;
        LoggerERura();
        ~LoggerERura();
        friend LoggerERura &operator << (LoggerERura &logger, const ERuraLogType logType);
        friend LoggerERura &operator << (LoggerERura &logger, const std::string text);

    private:
        //Name of file
        std::string loggerFileName = "eRuraLog.txt";

};


#endif //TIN_LOGGERERURA_H
