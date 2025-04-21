#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <cstdint>
#include <array>

#include <iostream>
#include <memory>
#include <array>
#include <functional>
#include <cstdint>

namespace soul {

// Node structure for the linked list in each bucket
template <typename T>
struct Node_t {
    T key;
    std::unique_ptr<Node_t<T>> next;
    
    Node_t(T k) : key(k), next(nullptr) {}
};

// Thomas Wang's 64-bit to 32-bit hash function
struct ThomasWangHash {
    constexpr uint32_t operator()(uint64_t key) const {
        key = (~key) + (key << 18); // key = (key << 18) - key - 1;
        key = key ^ (key >> 31);
        key = key * 21; // key = (key + (key << 2)) + (key << 4);
        key = key ^ (key >> 11);
        key = key + (key << 6);
        key = key ^ (key >> 22);
        return static_cast<uint32_t>(key);
    }
};

// HashSet templateclass
template <typename T, typename Hash = ThomasWangHash, typename KeyEqual = std::equal_to<T> >
class HashSet_t {
public:
    constexpr explicit HashSet_t(float p_loadFactor = defaultLoadFactor) 
        : elementCount(0), currentPrimeIndex(0) {
        loadFactor = p_loadFactor;
        bucketCount = primeSizes[currentPrimeIndex];
        buckets = std::make_unique<std::unique_ptr<Node_t<T>>[]>(bucketCount);
    }

    ~HashSet_t() = default;

    // Insert Key
    bool insert(const T& key) {    
        if (elementCount > bucketCount * loadFactor) {
            resize(); // Resize if load factor > x
        }

        uint64_t hashValue = getHash(key);
        Node_t<T>* current = buckets[hashValue].get();
    
        while (current != nullptr) {
            if (keyEqual(current->key, key)) {
                // std::cout << "Key already exists. Insertion skipped." << std::endl;
                return false;
            }
            current = current->next.get();
        }

        auto newNode = std::make_unique<Node_t<T>>(key);
        newNode->next = std::move(buckets[hashValue]);
        buckets[hashValue] = std::move(newNode);
        ++elementCount;
        // std::cout << "Key inserted." << std::endl;
        return true;
    }

    // Search Key
    bool search(const T& key) {
        uint64_t hashValue = getHash(key);
        Node_t<T>* current = buckets[hashValue].get();
        
        while (current != nullptr) {
            if (keyEqual(current->key, key)) {
                // std::cout << "Key found." << std::endl;
                return true;
            }
            current = current->next.get();
        }
        
        // std::cout << "Key not found." << std::endl;
        return false;
    }

    // Remove Key
    bool remove(const T& key) {
        uint64_t hashValue = getHash(key);
        Node_t<T>* current = buckets[hashValue].get();
        Node_t<T>* prev = nullptr;
    
        while (current != nullptr) {
            if (keyEqual(current->key, key)) {
                if (prev == nullptr) {
                    buckets[hashValue] = std::move(current->next);
                } else {
                    prev->next = std::move(current->next);
                }
                // std::cout << "Key removed." << std::endl;
                --elementCount;
                return true;
            }
            prev = current;
            current = current->next.get();
        }
        // std::cout << "Key not found. Removal skipped." << std::endl;
        return false;
    }

    // Display HashSet
    void display() {
        std::cout << "HashSet contents:" << std::endl;
        for (uint64_t i = 0; i < bucketCount; ++i) {
            std::cout << "Bucket " << i << ": ";
            Node_t<T>* current = buckets[i].get();
            while (current != nullptr) {
                std::cout << current->key << " -> ";
                current = current->next.get();
            }
            std::cout << "nullptr" << std::endl;
        }
    }

    uint64_t getCountElements() const { return elementCount; }

private:
    // Get hash value for a key
    uint64_t getHash(const T& key) const {
        return hasher(key) % bucketCount;
    }

    // Resize function to move to the next prime size
    void resize() {
        if (currentPrimeIndex + 1 >= primeSizes.size()) return; // No more primes available

        int oldBucketCount = bucketCount;
        bucketCount = primeSizes[++currentPrimeIndex];

        auto newBuckets = std::make_unique<std::unique_ptr<Node_t<T>>[]>(bucketCount);

        for (int i = 0; i < oldBucketCount; ++i) {
            Node_t<T>* current = buckets[i].get();
            while (current) {
                uint64_t newHashValue = getHash(current->key);
                auto newNode = std::make_unique<Node_t<T>>(current->key);
                newNode->next = std::move(newBuckets[newHashValue]);
                newBuckets[newHashValue] = std::move(newNode);
                current = current->next.get();
            }
        }

        buckets = std::move(newBuckets);
    }

    uint64_t bucketCount;
    uint64_t elementCount;

    std::unique_ptr<std::unique_ptr<Node_t<T>>[]> buckets;

    double loadFactor; // Load factor variable

    // Static constexpr default load factor
    static constexpr double defaultLoadFactor = 0.7;

    static constexpr std::array<uint64_t, 33> primeSizes {
        11ULL, 23ULL, 47ULL, 97ULL, 199ULL, 409ULL, 823ULL, 1741ULL, 3469ULL, 6949ULL, 14033ULL,
        28067ULL, 56103ULL, 112213ULL, 224467ULL, 448949ULL, 897919ULL, 1795847ULL,
        3591703ULL, 7183417ULL, 14366889ULL, 28733777ULL, 57467521ULL, 114935069ULL,
        229870171ULL, 459740359ULL, 919480687ULL, 1838961469ULL, 3677922933ULL,
        7355845867ULL, 14711691733ULL, 29423383469ULL, 58846766941ULL
    }; // Example primes
    
    int currentPrimeIndex;

    Hash hasher;
    KeyEqual keyEqual;
};

}// namespace soul