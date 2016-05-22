//
// Created by jdudziak on 5/22/16.
//
// Class realize logger to file in all functions of program

#ifndef TIN_LOGGERERURA_H
#define TIN_LOGGERERURA_H

#include "../logger/ERuraLogType.h"

class LoggerERura {

    public:

        LoggerERura();
        ~LoggerERura();
        friend LoggerERura &operator << (LoggerERura &logger, const eRuraLogType logType);
        friend LoggerERura &operator << (LoggerERura &logger, std::string);

    private:
        //Name of file
        const string loggerFileName = "eRuraLog.txt";
        std::ofstream  logFile;
};


#endif //TIN_LOGGERERURA_H
