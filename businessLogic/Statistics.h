//
// Created by Jakub Dudziak.
//
// Class - statistical methods.

#ifndef TIN_STATISTICS_H
#define TIN_STATISTICS_H

#include "../manager/HistoryRecord.h"
#include <vector>

class Statistics{

    public:

    double countAverageCurrentResistance(std::vector<HistoryRecord*>* History);
    double countAverageTypicalResistance(std::vector<HistoryRecord*>* History);
    double countAverageDifferenceResistance(std::vector<HistoryRecord*>* History); // difference resistance
    int countDetectorWithAlarm(std::vector<HistoryRecord*>* History); // when Typical is higher then current
    int countDetectorWithInterruptNet(std::vector<HistoryRecord*>* History); // when Net is interrupt

    private:
    int static const INFINITY_RESISTANCE = 2;
};

#endif //TIN_STATISTICS_H
