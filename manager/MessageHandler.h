//
// Created by msacharc on 23.05.16.
//

#ifndef TIN_MESSAGEHANDLER_H
#define TIN_MESSAGEHANDLER_H

#include <cstdio>
#include "../common/message.h"
#include "../common/messageType.h"
#include "DetectorHistory.h"
#include "../logger/LoggerERura.h"


class MessageHandler {
private:
    DetectorHistory history;
    LoggerERura logger;
public:
    void handle(struct message* msg);
};


#endif //TIN_MESSAGEHANDLER_H
