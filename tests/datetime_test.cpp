
#include <gtest/gtest.h>
#include "datetime.h"
#include <iostream>

using namespace soul;
using namespace std;

// Test fixture to set up DateTime tests
class DateTimeTest : public ::testing::Test {
protected:
    DateTimeTest() : dt(2022, 1, 1) {}  // Initialize DateTime to a fixed date (2022-01-01)
    
    DateTime dt;  // The DateTime object we will be testing
};

// Demonstrate some basic assertions.
TEST_F(DateTimeTest, BasicAssertions) {

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

// Test the DateTime constructor and toString() method
TEST_F(DateTimeTest, ConstructorAndToString) {
    // Check that the date is initialized correctly
    EXPECT_EQ(dt.toString(), "2022-01-01 00:00:00");
}

// Test adding seconds to the DateTime object
TEST_F(DateTimeTest, AddSeconds) {
    dt.addSeconds(3600);  // Add 3600 seconds (1 hour)
    EXPECT_EQ(dt.toString(), "2022-01-01 01:00:00");
}

// Test subtracting seconds from the DateTime object
TEST_F(DateTimeTest, SubtractSeconds) {
    dt.subtractSeconds(3600);  // Subtract 3600 seconds (1 hour)
    EXPECT_EQ(dt.toString(), "2021-12-31 23:00:00");
}

// Test adding days to the DateTime object
TEST_F(DateTimeTest, AddDays) {
    dt.addDays(10);  // Add 10 days
    EXPECT_EQ(dt.toString(), "2022-01-11 00:00:00");
}

// Test subtracting days from the DateTime object
TEST_F(DateTimeTest, SubtractDays) {
    dt.subtractDays(10);  // Subtract 10 days
    EXPECT_EQ(dt.toString(), "2021-12-22 00:00:00");
}

// Test adding months to the DateTime object
TEST_F(DateTimeTest, AddMonths) {
    dt.addMonths(3);  // Add 3 months
    EXPECT_EQ(dt.toString(), "2022-04-01 01:00:00");
}

// Test subtracting months from the DateTime object
TEST_F(DateTimeTest, SubtractMonths) {
    dt.subtractMonths(3);  // Subtract 3 months
    EXPECT_EQ(dt.toString(), "2021-10-01 01:00:00");
}

// Test adding years to the DateTime object
TEST_F(DateTimeTest, AddYears) {
    dt.addYears(5);  // Add 5 years
    EXPECT_EQ(dt.toString(), "2027-01-01 00:00:00");
}

// Test subtracting years from the DateTime object
TEST_F(DateTimeTest, SubtractYears) {
    dt.subtractYears(5);  // Subtract 5 years
    EXPECT_EQ(dt.toString(), "2017-01-01 00:00:00");
}

// Test isBusinessDay method
TEST_F(DateTimeTest, IsBusinessDay) {
    // Check if a known business day is considered a business day
    dt.addDays(1);  // Move to January 2nd (2022-01-02)
    EXPECT_FALSE(dt.isBusinessDay());  // January 2nd is a Sunday

    dt.addDays(1);  // Move to January 3rd (2022-01-03)
    EXPECT_TRUE(dt.isBusinessDay());  // January 3rd is a Monday
}

// Test adding business days to DateTime object
TEST_F(DateTimeTest, AddBusinessDays) {
    dt.addBusinessDays(5);  // Add 5 business days
    EXPECT_EQ(dt.toString(), "2022-01-07 00:00:00");  // After adding business days
}

// Test subtracting business days from DateTime object
TEST_F(DateTimeTest, SubtractBusinessDays) {
    dt.subtractBusinessDays(5);  // Subtract 5 business days
    EXPECT_EQ(dt.toString(), "2021-12-27 00:00:00");  // Before subtracting business days
}

