#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include "singleton.h"

namespace soul {

// Dummy class for testing SingletonT
class TestSingleton : public SingletonT<TestSingleton> {
    MAKE_SINGLETON(TestSingleton)
public:
    int value = 0;  // Just for testing instance consistency
};

// Test that getInstance() returns the same instance
TEST(SingletonTest, ReturnsSameInstance) {
    TestSingleton& instance1 = TestSingleton::getInstance();
    TestSingleton& instance2 = TestSingleton::getInstance();
    
    EXPECT_EQ(&instance1, &instance2);
}

// Test thread safety
TEST(SingletonTest, ThreadSafety) {
    std::vector<std::thread> threads;
    std::vector<TestSingleton*> instances(10, nullptr);

    for (size_t i = 0; i < 10; ++i) {
        threads.emplace_back([&, i]() {
            instances[i] = &TestSingleton::getInstance();
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    for (size_t i = 1; i < instances.size(); ++i) {
        EXPECT_EQ(instances[i], instances[0]);  // All should be the same instance
    }
}

} // namespace soul
