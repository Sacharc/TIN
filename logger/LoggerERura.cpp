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
friend LoggerERura &operator << (LoggerERura &logger, const eRuraLogType logType) {

    switch (logType) {
        case LoggerERura::eRuraLogType::LOGGER_ERROR:
            logger.myFile << "[ERROR]: ";

        case LoggerERura::eRuraLogType::LOGGER_WARNING:
            logger.myFile << "[WARNING]: ";
            break;

        case LoggerERura::eRuraLogType::LOGGER_DEBUG:
            logger.myFile << "[DEBUG]: ";
            break;

        case LoggerERura::eRuraLogType::LOGGER_INFO:
            logger.myFile << "[INFO]: ";
            break;

        default:
            std::cout << "WRONG OPTION in using class loggerERURA, this type of log does not exist" << std::endl;
            break;
    }

    return logger;

}

//Overload operator "<<" without addnotation
friend LoggerERura &operator << (LoggerERura &logger, std::string text) {

    logger.myFile << text << std::endl;
    return logger;

}