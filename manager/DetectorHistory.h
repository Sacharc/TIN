//
// Created by msacharc on 07.05.16.
//

#ifndef TIN_DETECTORHISTORY_H
#define TIN_DETECTORHISTORY_H

#include<map>
#include<vector>
#include "HistoryRecord.h"

class DetectorHistory {
private:
    std::map<int, std::vector<HistoryRecord*>*> history;

public:
//    DetectorHistory() { };
    void add(int detectorId, HistoryRecord* record);
    std::vector<HistoryRecord*>* getRecords(int detectorId);
};


#endif //TIN_DETECTORHISTORY_H
