//
// Created by msacharc on 07.05.16.
//

#include "DetectorHistory.h"

void DetectorHistory::add(int detectorId, HistoryRecord* record) {
    // check if mamp contains key
    if(history.find(detectorId) == history.end()) {
        history[detectorId] = new std::vector<HistoryRecord*>;
    }
    history[detectorId]->push_back(record);
}

std::vector<HistoryRecord*>* DetectorHistory::getRecords(int detectorId) {
    // check if mamp contains key
    if(history.find(detectorId) == history.end()) {
        return nullptr;
    }
    return history[detectorId];
}

