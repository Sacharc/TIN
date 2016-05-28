//
// Created by msacharc on 23.05.16.
//
#include "MessageHandler.h"

void MessageHandler::handle(struct message *msg) {

    switch (msg->msg_type) {
        case REPORT:
            printf("Detector %ld reported: waterlevel: %d/%d\n", msg->id, msg->currentResistance,
                   msg->typicalResistance);
            break;
        case ALARM:
            printf("Detector %ld ALARMED: waterlevel: %d/%d\n", msg->id, msg->currentResistance,
                   msg->typicalResistance);
            break;
        deafult:
            printf("Unknown message from detector %ld\n", msg->id);
            break;
    }

    time_t  timev;
    time(&timev);
    history.add(msg->id, new HistoryRecord(*msg, timev));

}

DetectorHistory &MessageHandler::getHistory() {
    return history;
}


