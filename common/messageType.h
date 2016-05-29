//
// Created by Michal Sacharczuk.
//
#ifndef TEST_MESSAGETYPE_H
#define TEST_MESSAGETYPE_H

enum messageType {
    REPORT,
    ALARM,
    INFINITY_RESISTANCE,
    CHANGE_TYPICAL_RESISTANCE,
    CHECK_STATE
};

const char* messageTypeToString(int type);

#endif //TEST_MESSAGETYPE_H
