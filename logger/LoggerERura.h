//
// Created by jdudziak on 5/22/16.
//
// Class realize logger to file in all functions of program - singleton

#ifndef TIN_LOGGERERURA_H
#define TIN_LOGGERERURA_H

#include "../logger/ERuraLogType.h"
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

class LoggerERura {

    public:
        static LoggerERura& getInstance() {
            static LoggerERura instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }
        ~LoggerERura();
        LoggerERura(LoggerERura const&) = delete;
        void operator=(LoggerERura const&) = delete;
        std::ofstream logFile;

        friend LoggerERura &operator << (LoggerERura &logger, const ERuraLogType logType);
        friend LoggerERura &operator << (LoggerERura &logger, const std::string text);

    private:
        //Name of file
        LoggerERura();
        std::string loggerFileName = "eRuraLog.txt";

};


#endif //TIN_LOGGERERURA_H
