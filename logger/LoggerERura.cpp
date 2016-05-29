//
// Created by Jakub Dudziak
//

#include "LoggerERura.h"

//Constructor
LoggerERura::LoggerERura() {

    logFile.open (loggerFileName);

    if (logFile.is_open()) {
        logFile << "ERura Log History" << std::endl;
    }

}

//Destructor
LoggerERura::~LoggerERura() {

    if (logFile.is_open()) {
        logFile << std::endl << "End of File" << std::endl;
        logFile.close();
    }

}

// Overload opertor "<<" with addnotation
LoggerERura &operator << (LoggerERura &logger, const ERuraLogType logType) {

    time_t now;
    time(&now);

    switch (logType) {
        case ERuraLogType :: LOGGER_ERROR:
            logger.logFile << std::endl << ctime(&now) << "[ERROR]: " << std::endl;
            break;

        case ERuraLogType :: LOGGER_WARNING:
            logger.logFile << std::endl << ctime(&now)  << "[WARNING]: " << std::endl;
            break;

        case ERuraLogType :: LOGGER_DEBUG:
            logger.logFile << std::endl << ctime(&now) << "[DEBUG]: " << std::endl;
            break;

        case ERuraLogType :: LOGGER_INFO:
            logger.logFile << std::endl << ctime(&now)  << "[INFO]: " << std::endl;
            break;

        default:
            std::cout << "WRONG OPTION in using class loggerERURA, this type of log does not exist" << std::endl ;
            break;
    }

    return logger;

}

//Overload operator "<<" without addnotation
LoggerERura &operator << (LoggerERura &logger, const std::string text) {

    logger.logFile << text << std::endl;
    return logger;

}