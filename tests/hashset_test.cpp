#include <gtest/gtest.h>
#include "hashset.h"

namespace soul {

TEST(HashSetTest, InsertAndSearch) {
    HashSet_t<int> hashSet;
    EXPECT_TRUE(hashSet.insert(10));
    EXPECT_TRUE(hashSet.insert(20));
    EXPECT_TRUE(hashSet.insert(30));
    EXPECT_FALSE(hashSet.insert(10)); // Duplicate

    EXPECT_TRUE(hashSet.search(10));
    EXPECT_TRUE(hashSet.search(20));
    EXPECT_TRUE(hashSet.search(30));
    EXPECT_FALSE(hashSet.search(40)); // Non-existent
}

TEST(HashSetTest, Remove) {
    HashSet_t<int> hashSet;
    hashSet.insert(10);
    hashSet.insert(20);
    hashSet.insert(30);
    
    EXPECT_TRUE(hashSet.remove(20));
    EXPECT_FALSE(hashSet.search(20));
    EXPECT_FALSE(hashSet.remove(40)); // Removing non-existent element
}

TEST(HashSetTest, Resize) {
    HashSet_t<int> hashSet(0.5f); // Lower load factor to trigger resize sooner
    for (int i = 1; i <= 50; ++i) {
        hashSet.insert(i);
    }
    
    EXPECT_TRUE(hashSet.search(25));
    EXPECT_TRUE(hashSet.search(50));
    EXPECT_FALSE(hashSet.search(100));
}

TEST(HashSetTest, EdgeCases) {
    HashSet_t<int> hashSet;
    EXPECT_FALSE(hashSet.remove(99)); // Removing from empty set
    EXPECT_FALSE(hashSet.search(1000)); // Searching empty set
}

TEST(HashSetTest, CollisionHandling) {
    HashSet_t<int> hashSet;
    int key1 = 10, key2 = 21; // Assuming they hash to the same bucket
    hashSet.insert(key1);
    hashSet.insert(key2);

    EXPECT_TRUE(hashSet.search(key1));
    EXPECT_TRUE(hashSet.search(key2));
}

TEST(HashSetTest, LargeDatasetPerformance) {
    HashSet_t<int> hashSet;
    constexpr int largeSize = 100000;
    for (int i = 0; i < largeSize; ++i) {
        hashSet.insert(i);
    }
    
    for (int i = 0; i < largeSize; ++i) {
        EXPECT_TRUE(hashSet.search(i));
    }
    
    EXPECT_EQ(hashSet.getCountElements(), largeSize);
}

TEST(HashSetTest, LargeDatasetPerformance2) {
    HashSet_t<int> hashSet;
    constexpr int largeSize = 100000;
    
    auto startInsert = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < largeSize; ++i) {
        hashSet.insert(i);
    }
    auto endInsert = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> insertDuration = endInsert - startInsert;
    std::cout << "Insertion time: " << insertDuration.count() << " seconds" << std::endl;
    
    auto startSearch = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < largeSize; ++i) {
        EXPECT_TRUE(hashSet.search(i));
    }
    auto endSearch = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> searchDuration = endSearch - startSearch;
    std::cout << "Search time: " << searchDuration.count() << " seconds" << std::endl;
    
    EXPECT_EQ(hashSet.getCountElements(), largeSize);
}

} // namespace soul
