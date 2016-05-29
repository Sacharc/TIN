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
    allRecords.push_back(record);
}

std::vector<HistoryRecord*>* DetectorHistory::getRecords(int detectorId) {
    // check if mamp contains key
    if(history.find(detectorId) == history.end()) {
        return new std::vector<HistoryRecord*>;
    }
    return history[detectorId];
}

std::vector<HistoryRecord*>* DetectorHistory::getAllRecords() {
//    std::vector<HistoryRecord*>* vec = new std::vector<HistoryRecord*>;
//    for(auto it = history.begin(); it!= history.end(); it++) {
//        auto tempVec = it->second;
//        vec->insert(vec->end(), tempVec->begin(), tempVec->end());
//    }
    return &allRecords;
}

std::vector<int> DetectorHistory::getDetectorIds() {
    std::vector<int> vec;
    for(auto it = history.begin(); it!= history.end(); it++) {
        vec.push_back(it->first);
    }
    return vec;
}

void DetectorHistory::clearHistory() {
    allRecords.clear();
    history.clear();
}