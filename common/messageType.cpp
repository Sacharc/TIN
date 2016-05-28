//
// Created by sacharc on 28.05.16.
//

#include "messageType.h"

const char* messageTypeToString(int type) {
    switch(type) {
        case REPORT:
            return "Report";
        case ALARM:
            return "Alarm";
        case INFINITY_RESISTANCE:
            return "Infinity resistance";
        case CHANGE_TYPICAL_RESISTANCE:
            return "Change typical resistance";
        case CHECK_STATE:
            return "Check state";
        default:
            return "";
    }
}
