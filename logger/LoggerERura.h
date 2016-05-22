//
// Created by jdudziak on 5/22/16.
//
// Class realize logger to file in all functions of program

#ifndef TIN_LOGGERERURA_H
#define TIN_LOGGERERURA_H


class LoggerERura {

    public:

        enum eRuraLogType { LOGGER_ERROR, LOGGER_WARNING, LOGGER_INFO, LOGGER_DEBUG};
        LoggerERura();
        ~LoggerERura();
        friend LoggerERura &operator << (FileLogger &logger, const e_logType l_type);
        friend LoggerERura &operator << (FileLogger &logger, std::string);

    private:
        //Name of file
        const string loggerFileName = "eRuraLog.txt";
        std::ofstream  logFile;
};


#endif //TIN_LOGGERERURA_H
