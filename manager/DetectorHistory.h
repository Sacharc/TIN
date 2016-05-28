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
    void add(int detectorId, HistoryRecord* record);
    std::vector<HistoryRecord*>* getRecords(int detectorId);
    std::vector<HistoryRecord*>* getAllRecords();
    std::vector<int> getDetectorIds();
    void clearHistory();
};

#endif //TIN_DETECTORHISTORY_H
