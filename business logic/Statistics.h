//
// Created by jdudziak on 5/8/16.
//
// Class - statistical methods.

#ifndef TIN_STATISTICS_H
#define TIN_STATISTICS_H

#include "../manager/HistoryRecord.h"
#include<vector>

class Statistics{

    public:

    double countAverageResistanceOneDetector(std::vector<HistoryRecord>* History);
};

#endif //TIN_STATISTICS_H
