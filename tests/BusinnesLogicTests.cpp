//
// Created by jdudziak on 5/27/16.
//

#include <iostream>
#include <gtest/gtest.h>
#include "../manager/DetectorHistory.h"
#include "../businessLogic/Statistics.h"

TEST(Test_Statistica_1, PositiveNos) {
    message a[5];
    int temp_value_of_typical = 1001;
    time_t now;

    std::vector<HistoryRecord*>* history = new std::vector<HistoryRecord*>;

    a[0].currentResistance = 1000;
    a[1].currentResistance = 1023;
    a[2].currentResistance = 1032;
    a[3].currentResistance = 1011;
    a[4].currentResistance = 1011;

    for(int i=0;i<5;i++) {
        a[i].id = i+1;
        a[i].msg_type = 1;
        a[i].typicalResistance = temp_value_of_typical+i;
        auto historyRecord = new HistoryRecord(a[i],time(&now));
        history->push_back(historyRecord);
    }

    Statistics statistics;

    ASSERT_EQ(statistics.countAverageCurrentResistance(history), 1015.4);
    ASSERT_EQ(statistics.countAverageTypicalResistance(history), 1003);
    ASSERT_EQ(statistics.countAverageDifferenceResistance(history), 12.4);
    ASSERT_EQ(statistics.countDetectorWithAlarm(history), 1);
    ASSERT_EQ(statistics.countDetectorWithInterruptNet(history), 0);
}

TEST(Test_Statistica_2, PositiveNos) {
    message a[4];
    int temp_value_of_typical = 990;
    time_t now;

    std::vector<HistoryRecord*>* history = new std::vector<HistoryRecord*>;

    a[0].currentResistance=1020;
    a[1].currentResistance=1025;
    a[2].currentResistance=1040;
    a[3].currentResistance=1000;
    a[0].msg_type=2;
    a[1].msg_type=1;
    a[2].msg_type=2;
    a[3].msg_type=0;

    for(int i=0;i<4;i++) {
        a[i].id = i+1;
        a[i].typicalResistance=temp_value_of_typical-10*i;
        auto historyRecord = new HistoryRecord(a[i],time(&now));
        history->push_back(historyRecord);
    }

    Statistics statistics;

    ASSERT_EQ(statistics.countAverageCurrentResistance(history), 1012.5);
    ASSERT_EQ(statistics.countAverageTypicalResistance(history), 970);
    ASSERT_EQ(statistics.countAverageDifferenceResistance(history), 42.5);
    ASSERT_EQ(statistics.countDetectorWithAlarm(history), 0);
    ASSERT_EQ(statistics.countDetectorWithInterruptNet(history), 2);
}

TEST(Test_Statistica_3, PositiveNos) {
    message a[5];
    int temp_value_of_typical = 1100;
    time_t now;

    std::vector<HistoryRecord*>* history = new std::vector<HistoryRecord*>;

    a[0].currentResistance=1050;
    a[1].currentResistance=1034;
    a[2].currentResistance=1000;
    a[3].currentResistance=987;
    a[4].currentResistance=1150;
    a[0].msg_type=2;
    a[1].msg_type=1;
    a[2].msg_type=2;
    a[3].msg_type=0;
    a[4].msg_type=1;

    for(int i=0;i<5;i++) {
        a[i].id = i+1;
        a[i].typicalResistance=temp_value_of_typical;
        auto historyRecord = new HistoryRecord(a[i],time(&now));
        history->push_back(historyRecord);
    }

    Statistics statistics;

    ASSERT_EQ(statistics.countAverageTypicalResistance(history), 1100);
    ASSERT_EQ(statistics.countAverageDifferenceResistance(history), -43);
    ASSERT_EQ(statistics.countDetectorWithAlarm(history), 2);
    ASSERT_EQ(statistics.countDetectorWithInterruptNet(history), 2);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


