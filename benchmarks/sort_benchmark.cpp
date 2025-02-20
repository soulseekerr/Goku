#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <random>

// Generate random array of integers
std::vector<int> generate_random_vector(size_t size) {
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10000);
    
    for (auto& num : vec) {
        num = dist(gen);
    }
    return vec;
}

/**
 * Bubble Sort - O(n²)
 * -----------------------------------------
 * Repeatedly swaps adjacent elements if they are in the wrong order.
 * This results in the largest element "bubbling" to the top.
 * Very slow for large datasets.
 */
void bubble_sort(std::vector<int>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

/**
 * Quick Sort - O(n log n) (average case)
 * -----------------------------------------
 * Uses a divide-and-conquer strategy:
 * 1. Selects a "pivot" element.
 * 2. Partitions the array so that elements smaller than the pivot are on the left, and larger ones are on the right.
 * 3. Recursively sorts the left and right partitions.
 * Fast in practice but can degrade to O(n²) in the worst case (e.g., already sorted input with bad pivot choice).
 */
void quick_sort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int pivot = arr[right];
    int partition_index = left;
    
    for (int i = left; i < right; ++i) {
        if (arr[i] < pivot) {
            std::swap(arr[i], arr[partition_index]);
            partition_index++;
        }
    }
    std::swap(arr[partition_index], arr[right]);
    quick_sort(arr, left, partition_index - 1);
    quick_sort(arr, partition_index + 1, right);
}

// Wrapper for Quick Sort
void quick_sort_wrapper(std::vector<int>& arr) {
    quick_sort(arr, 0, arr.size() - 1);
}

/**
 * std::sort (Introsort) - O(n log n) (worst case)
 * -----------------------------------------
 * A hybrid sorting algorithm combining:
 * - Quick Sort
 * - Heap Sort (for worst-case Quick Sort scenarios)
 * - Insertion Sort (for small arrays)
 * It is highly optimized and usually outperforms manual Quick Sort implementations.
 */

// Benchmark Bubble Sort
static void BM_BubbleSort(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec = generate_random_vector(state.range(0));
        bubble_sort(vec);
    }
}
BENCHMARK(BM_BubbleSort)->Range(8, 2048);
BENCHMARK(BM_BubbleSort)->Range(16384, 16384);

// Benchmark Quick Sort
static void BM_QuickSort(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec = generate_random_vector(state.range(0));
        quick_sort_wrapper(vec);
    }
}
BENCHMARK(BM_QuickSort)->Range(8, 2048);
BENCHMARK(BM_QuickSort)->Range(16384, 16384);

// Benchmark std::sort
static void BM_StdSort(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec = generate_random_vector(state.range(0));
        std::sort(vec.begin(), vec.end());
    }
}
BENCHMARK(BM_StdSort)->Range(8, 2048);
BENCHMARK(BM_StdSort)->Range(16384, 16384);
