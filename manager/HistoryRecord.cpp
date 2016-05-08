//
// Created by msacharc on 07.05.16.
//

#include "HistoryRecord.h"

HistoryRecord::HistoryRecord(struct message &msg, time_t time) :message(msg), time(time) { }

struct message& HistoryRecord::getMessage() {
    return message;
}

time_t HistoryRecord::getTime() {
    return time;
}

int HistoryRecord::getCurrentResistance() {
    return getMessage().currentResistance;
}

int HistoryRecord::getTypicalResistance() {
    return getMessage().typicalResistance;
}