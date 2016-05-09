//
// Created by jdudziak on 5/8/16.
//


#include "Statistics.h"

Statistics::Statistics() { }

double Statistics::countAverageResistanceOneDetector(std::vector<HistoryRecord>* History){
    double sum=0;
    std::vector <HistoryRecord>::iterator it;

    for(auto it = History.begin(); it != History.end(); ++it) {
        HistoryRecord abc = *(*it);
        sum = sum + abc.getCurrentResistance();
    }

    return sum/History->size();
}