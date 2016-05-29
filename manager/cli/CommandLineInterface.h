//
// Created by Michał Sacharczuk
//

#ifndef TIN_COMMANDLINEINTERFACE_H
#define TIN_COMMANDLINEINTERFACE_H


#include <mutex>
#include "../DetectorHistory.h"
#include <iostream>
#include <algorithm>
#include "../../common/messageType.h"
#include "../../businessLogic/Statistics.h"

class CommandLineInterface {
private:
    DetectorHistory* history;
    std::mutex* m;
    bool end;
public:
    CommandLineInterface(DetectorHistory* hist, std::mutex* mut);
    bool isEnd();
    void mainMenu();
private:
    void displayDetectorList();
    void displayStatistics();
    bool chooseDetector(int &value);
    void displayHistory(bool alarmsOnly);
    void clearHistory();
    void findDetectors();
    void changeTypicalResistance();
};


#endif //TIN_COMMANDLINEINTERFACE_H
