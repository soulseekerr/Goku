#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include "core/core.h"

// --- SafeNumeric Tests ---
TEST(SafeNumericTest, BasicOperations) {
    SafeNumeric<int> num(10);
    
    EXPECT_EQ(num.get(), 10);
    
    num.set(20);
    EXPECT_EQ(num.get(), 20);

    EXPECT_EQ(num.increment(), 21);
    EXPECT_EQ(num.decrement(), 20);

    EXPECT_EQ(num.add(5), 25);
    EXPECT_EQ(num.sub(3), 22);
}

TEST(SafeNumericTest, AtomicIncrementsMultiThreaded) {
    SafeNumeric<int> num(0);
    constexpr int THREADS = 10;
    constexpr int INCREMENTS = 1000;

    auto increment_task = [&]() {
        for (int i = 0; i < INCREMENTS; ++i) {
            num.increment();
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(increment_task);
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(num.get(), THREADS * INCREMENTS);
}

// --- SafeFlag Tests ---
TEST(SafeFlagTest, SetClearCheck) {
    SafeFlag flag;

    EXPECT_FALSE(flag.is_set());

    flag.set();
    EXPECT_TRUE(flag.is_set());

    flag.clear();
    EXPECT_FALSE(flag.is_set());
}

TEST(SafeFlagTest, MultiThreadedToggle) {
    SafeFlag flag;
    constexpr int THREADS = 10;

    auto toggle_task = [&]() {
        flag.set();
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(toggle_task);
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_TRUE(flag.is_set()); // At least one thread must have set it.
}
