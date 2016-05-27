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

    a[0].currentResistance=1000;
    a[1].currentResistance=1023;
    a[2].currentResistance=1032;
    a[3].currentResistance=1011;
    a[4].currentResistance=1011;

    for(int i=0;i<5;i++) {
        a[i].id=i+1;
        a[i].msg_type=1;
        a[i].typicalResistance=temp_value_of_typical+i;
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

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


