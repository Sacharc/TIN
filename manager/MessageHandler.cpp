//
// Created by Michal Sacharczuk.
//

#include "MessageHandler.h"

void MessageHandler::handle(struct message *msg) {
    char buf[100];

    switch (msg->msg_type) {
        case REPORT:
            snprintf(buf, 100, "Detector %ld reported: waterlevel: %d/%d\n", msg->id, msg->currentResistance,
                   msg->typicalResistance);
            LoggerERura::getInstance() << ERuraLogType::LOGGER_INFO;
            LoggerERura::getInstance() << buf;
            break;
        case ALARM:
            snprintf(buf, 100, "Detector %ld ALARMED: waterlevel: %d/%d\n", msg->id, msg->currentResistance,
                   msg->typicalResistance);
            LoggerERura::getInstance() << ERuraLogType::LOGGER_WARNING;
            LoggerERura::getInstance() << buf;
            break;
        deafult:
            snprintf(buf, 100, "Unknown message from detector %ld\n", msg->id);
            break;
    }

    time_t  timev;
    time(&timev);
    history.add(msg->id, new HistoryRecord(*msg, timev));

}

DetectorHistory &MessageHandler::getHistory() {
    return history;
}


