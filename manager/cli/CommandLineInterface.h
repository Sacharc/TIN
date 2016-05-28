//
// Created by sacharc on 28.05.16.
//

#ifndef TIN_COMMANDLINEINTERFACE_H
#define TIN_COMMANDLINEINTERFACE_H


#include "../DetectorHistory.h"

class CommandLineInterface {
private:
    DetectorHistory* history;

public:
    CommandLineInterface(DetectorHistory* hist);
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
