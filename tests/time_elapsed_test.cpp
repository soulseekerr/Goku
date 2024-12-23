
#include <gtest/gtest.h>
#include "core/time_elapsed.h"
#include <iostream>
#include <thread>


// Demonstrate some basic assertions.
TEST(TimeElapsedTest, BasicAssertions) {
    soul::st_time_elapsed tp;
    tp.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(1400));

    tp.print_elapsed_ms();
    tp.print_elapsed_ns();
    tp.print_elapsed_s();

    soul::st_datetime dt;

    std::cout << dt.year_month_day_h() << std::endl;
}