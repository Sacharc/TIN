//
// Created by jdudziak on 5/22/16.
//

#include "LoggerERura.h"

//Constructor
LoggerERura::LoggerERura() {

    logFile.open (loggerFileName);

    if (logFile.is_open()) {
        logFile << "ERura Log History" << std::endl << std::endl;
    }

}

//Destructor
LoggerERura::~LoggerERura() {

    if (logFile.is_open()) {
        logFile << std::endl << std::endl << "End of File" << std::endl;
        logFile.close();
    }

}

// Overload opertor "<<" with addnotation
LoggerERura &operator << (LoggerERura &logger, const ERuraLogType logType) {

    switch (logType) {
        case ERuraLogType :: LOGGER_ERROR:
            logger.logFile << "[ERROR]: ";

        case ERuraLogType :: LOGGER_WARNING:
            logger.logFile << "[WARNING]: ";
            break;

        case ERuraLogType :: LOGGER_DEBUG:
            logger.logFile << "[DEBUG]: ";
            break;

        case ERuraLogType :: LOGGER_INFO:
            logger.logFile << "[INFO]: ";
            break;

        default:
            std::cout << "WRONG OPTION in using class loggerERURA, this type of log does not exist" << std::endl;
            break;
    }

    return logger;

}

//Overload operator "<<" without addnotation
LoggerERura &operator << (LoggerERura &logger, std::string text) {

    logger.logFile << text << std::endl;
    return logger;

}