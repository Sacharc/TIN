//
// Created by Michał Sacharczuk
//

#include "CommandLineInterface.h"

CommandLineInterface::CommandLineInterface(DetectorHistory *hist, std::mutex* mut) : history(hist),
                                                                                     m(mut),
                                                                                     end(false){ }

void CommandLineInterface::mainMenu() {
    std::vector<std::string> mainMenu;
    mainMenu.push_back("Wyswietl liste czujnikow");
    mainMenu.push_back("Wyswietl historie");
    mainMenu.push_back("Wyczysc historie");
    mainMenu.push_back("Wyswietl obecne alarmy");
    mainMenu.push_back("Statystyki");
    mainMenu.push_back("Zakoncz");

    while(1) {
        for (unsigned i = 0; i < mainMenu.size(); i++) {
            std::cout << i+1 << " " << mainMenu[i] << std::endl;
        }
        int val = 0;
        std::cin >> val;

        switch (val) {
            case 1:
                displayDetectorList();
                break;
            case 2:
                displayHistory(false);
                break;
            case 3:
                clearHistory();
                break;
            case 4:
                displayHistory(true);
                break;
            case 5:
                displayStatistics();
                break;
            case 6:
                m->lock();
                end = true;
                m->unlock();
                return;
            default:
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
        }
    }
}

void CommandLineInterface::displayDetectorList() {
    auto vec = history->getDetectorIds();
    for(unsigned i = 0; i< vec.size(); ++i) {
        std::cout<<i<<". "<<vec[i]<<std::endl;
    }
}

void CommandLineInterface::displayHistory(bool alarmsOnly) {
    auto vec = history->getAllRecords();
    size_t limit = vec->size();
    for (unsigned i = 0; i < limit; ++i) {
        char buff[80];
        if(alarmsOnly && (vec->at(i)->getMessageType() != ALARM))
            continue;
        std::string type = messageTypeToString(vec->at(i)->getMessageType());
        time_t time1 = vec->at(i)->getTime();
        strftime(buff, 80, "%Y-%m-%d %H:%M:%S", localtime(&time1));
        std::string time(buff);
        std::string currentResistence = std::to_string(vec->at(i)->getCurrentResistance());
        std::string typicalResistance = std::to_string(vec->at(i)->getTypicalResistance());

        std::cout << time
                <<"\t ID" << vec->at(i)->getMessage().id
                <<"\t type " << type
                <<"\t current: " << currentResistence
                <<"\t typical: " << typicalResistance << std::endl;
    }
}

void CommandLineInterface::clearHistory() {
    history->clearHistory();
}

bool CommandLineInterface::isEnd() {
    return end;
}

void CommandLineInterface::displayStatistics() {
    Statistics statistics;
    auto vec = history->getAllRecords();
    std::cout <<"Srednia rezystancja "                << statistics.countAverageCurrentResistance(vec) << std::endl;
    std::cout <<"Srednie odstepstwo "                 << statistics.countAverageDifferenceResistance(vec) << std::endl;
    std::cout <<"Srednia typowa rezystancja "         << statistics.countAverageTypicalResistance(vec) << std::endl;
    std::cout <<"Liczba alarmow "                     << statistics.countDetectorWithAlarm(vec) << std::endl;
    std::cout <<"Liczba zerwanych kabli oporowych "   << statistics.countDetectorWithInterruptNet(vec) << std::endl;
}