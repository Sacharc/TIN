//
// Created by Michał Sacharczuk
//

#include "CommandLineInterface.h"

void scanNetwork(std::mutex* m);

CommandLineInterface::CommandLineInterface(DetectorHistory *hist, std::mutex* mut, std::vector<int>* typ) : history(hist),
                                                                                                            m(mut),
                                                                                                            end(false),
                                                                                                            typical(typ) { }

void CommandLineInterface::mainMenu() {
    std::vector<std::string> mainMenu;
    mainMenu.push_back("Wyswietl liste czujnikow");
    mainMenu.push_back("Zmien poziom alarmow");
    mainMenu.push_back("Wyswietl historie");
    mainMenu.push_back("Wyczysc historie");
    mainMenu.push_back("Wyswietl obecne alarmy");
    mainMenu.push_back("Wyszukaj wszystkie czujniki w podsieci");
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
                changeTypicalResistance();
                break;
            case 3:
                displayHistory(false);
                break;
            case 4:
                clearHistory();
                break;
            case 5:
                displayHistory(true);
                break;
            case 6:
                findDetectors();
                break;
            case 7:
                displayStatistics();
                break;
            case 8:
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

void CommandLineInterface::findDetectors() {
    scanNetwork(m);
}

void CommandLineInterface::clearHistory() {
    history->clearHistory();
}

bool CommandLineInterface::chooseDetector(unsigned &value) {
    std::cout << "Lista czujnikow" <<std::endl;
    displayDetectorList();

    unsigned val = 0;
    std::cin >> val;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    auto vec = history->getDetectorIds();
    value = val;
    return (val >= 0 && val < vec.size());
}

void CommandLineInterface::changeTypicalResistance() {
    std::cout <<"Wybierz czujke: " << std::endl;
    unsigned value;
    if(chooseDetector(value)) {
        int resistance;
        std::cout << "Podaj rezystancje: " << std::endl;
        std::cin >> resistance;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Wystapil blad: " << std::endl;
            return;
        }
        typical->at(value) = resistance;
    } else {
        std::cout <<"Wystapil blad: " << std::endl;
    }

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