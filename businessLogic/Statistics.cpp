//
// Created by jdudziak on 5/8/16.
//


#include "Statistics.h"

double Statistics::countAverageCurrentResistance(std::vector<HistoryRecord*>* History) {

    double sum=0;
    int counterWorksFeller=0;

    for(auto it = History->begin(); it != History->end(); ++it) {
        if((*it)->getMessageType() != INFINITY_RESISTANCE) {
            sum = sum + (*it)->getCurrentResistance();
            counterWorksFeller++;
        }
    }

    return sum/counterWorksFeller;
}

double Statistics::countAverageTypicalResistance(std::vector<HistoryRecord*>* History) {

    double sum=0;
    int counterWorksFeller=0;

    for(auto it = History->begin(); it != History->end(); ++it) {
        if((*it)->getMessageType() != INFINITY_RESISTANCE) {
            sum = sum + (*it)->getTypicalResistance();
            counterWorksFeller++;
        }
    }

    return sum/counterWorksFeller;
}

double Statistics::countAverageDifferenceResistance(std::vector<HistoryRecord*>* History) {
    double sum=0;
    int counterWorksFeller=0;

    for(auto it = History->begin(); it != History->end(); ++it) {
        if((*it)->getMessageType() != INFINITY_RESISTANCE) {
            sum = sum + (*it)->getCurrentResistance() - (*it)->getTypicalResistance();
            counterWorksFeller++;
        }
    }

    return sum/counterWorksFeller;
}

int Statistics::countDetectorWithAlarm(std::vector<HistoryRecord*>* History) {
    int counter=0;

    for(auto it = History->begin(); it != History->end(); ++it) {
        if((*it)->getMessageType() != INFINITY_RESISTANCE && (*it)->getTypicalResistance() > (*it)->getCurrentResistance()){
            counter++;
        }
    }

    return counter;
}

int Statistics::countDetectorWithInterruptNet(std::vector<HistoryRecord *> *History) {
    int counter=0;

    for(auto it = History->begin(); it != History->end(); ++it) {
        if((*it)->getMessageType() == INFINITY_RESISTANCE){
            counter++;
        }
    }

    return counter;
}