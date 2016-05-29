//
// Created by Michal Sacharczuk.
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
public:
    void handle(struct message* msg);
    DetectorHistory &getHistory();
};


#endif //TIN_MESSAGEHANDLER_H
