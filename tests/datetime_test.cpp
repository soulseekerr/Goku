
#include <gtest/gtest.h>
#include "core/datetime.h"
#include <iostream>

using namespace soul;
using namespace std;

// Demonstrate some basic assertions.
TEST(DateTimeTest, BasicAssertions) {

    DateTime dt_1(2024, 12, 29);
    DateTime dt_2(2024, 8, 20);
    DateTime dt_3(2023, 2, 4);
    DateTime dt_4(2022, 4, 14);

    cout << "DateTime1: " << dt_1.year_month_day_h() << endl;
    cout << "DateTime2: " << dt_2.year_month_day() << endl;
    cout << "DateTime3: " << dt_3.year_month_h() << endl;
    cout << "DateTime4: " << dt_4.year() << endl;

    EXPECT_EQ(dt_1.year_month_day_h(), "2024-12-29");
    EXPECT_EQ(dt_2.year_month_day(), "20240820");
    EXPECT_EQ(dt_3.year_month(), "202302");
    EXPECT_EQ(dt_4.year(), "2022");
}