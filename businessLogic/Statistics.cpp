//
// Created by jdudziak on 5/8/16.
//


#include "Statistics.h"

double Statistics::countAverageResistanceOneDetector(std::vector<HistoryRecord>* History){
    double sum=0;

    for(auto it = History->begin(); it != History->end(); ++it) {
        sum = sum + it->getCurrentResistance();
    }

    return sum/History->size();
}