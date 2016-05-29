//
// Created by sacharc on 28.05.16.
//

#ifndef TIN_COMMANDLINEINTERFACE_H
#define TIN_COMMANDLINEINTERFACE_H


#include <mutex>
#include "../DetectorHistory.h"

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
    void chooseDetector();
    void displayHistory(bool alarmsOnly);
    void clearHistory();
    void findDetectors();
    void changeTypicalResistance();
};


#endif //TIN_COMMANDLINEINTERFACE_H
