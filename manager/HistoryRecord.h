//
// Created by msacharc on 07.05.16.
//

#ifndef TIN_HISTORYRECORD_H
#define TIN_HISTORYRECORD_H

#include <ctime>
#include "../common/message.h"

class HistoryRecord {
private:
    struct message message;
    time_t time;

public:
    HistoryRecord(struct message &msg, time_t time);

    struct message& getMessage();
    time_t getTime();
    int getCurrentResistance();
    int getTypicalResistance();
};


#endif //TIN_HISTORYRECORD_H
