//
// Created by Michal Sacharczuk.
//

#ifndef TIN_DETECTORHISTORY_H
#define TIN_DETECTORHISTORY_H

#include <map>
#include <vector>
#include "HistoryRecord.h"

class DetectorHistory {
private:
    std::map<int, std::vector<HistoryRecord*>*> history;
    std::vector<HistoryRecord*> allRecords;
public:
    void add(int detectorId, HistoryRecord* record);
    std::vector<HistoryRecord*>* getRecords(int detectorId);
    std::vector<HistoryRecord*>* getAllRecords();
    std::vector<int> getDetectorIds();
    void clearHistory();
    ~DetectorHistory();
};

#endif //TIN_DETECTORHISTORY_H
