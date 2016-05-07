#ifndef TEST_MESSAGETYPE_H
#define TEST_MESSAGETYPE_H

enum messageType {
    REPORT,
    ALARM,
    init = 0,
    callAlarm,
    getCurrentValue,
    setAlarmValue
};

#endif //TEST_MESSAGETYPE_H
