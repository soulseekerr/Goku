#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include <algorithm>
#include <future>

// Generate random data
std::vector<int> mt_generate_random_vector(int size) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100000);
    for (auto& x : arr) x = dist(gen);
    return arr;
}

// Sequential QuickSort
void mt_quick_sort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int pivot = arr[right], i = left;
    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) std::swap(arr[i++], arr[j]);
    }
    std::swap(arr[i], arr[right]);

    mt_quick_sort(arr, left, i - 1);
    mt_quick_sort(arr, i + 1, right);
}

// Parallel QuickSort
void mt_parallel_quick_sort(std::vector<int>& arr, int left, int right, int depth = 0) {
    if (left >= right) return;
    int pivot = arr[right], i = left;
    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) std::swap(arr[i++], arr[j]);
    }
    std::swap(arr[i], arr[right]);

    if (depth < 3) { // Limit recursion depth for thread spawning
        auto future_left = std::async(std::launch::async, mt_parallel_quick_sort, std::ref(arr), left, i - 1, depth + 1);
        mt_parallel_quick_sort(arr, i + 1, right, depth + 1);
        future_left.wait(); // Ensure left side is finished before continuing
    } else {
        mt_quick_sort(arr, left, i - 1);
        mt_quick_sort(arr, i + 1, right);
    }
}

// Benchmark Sequential QuickSort
static void BM_QuickSort(benchmark::State& state) {
    std::vector<int> arr = mt_generate_random_vector(state.range(0));
    for (auto _ : state) {
        std::vector<int> temp = arr;  // Copy before sorting
        mt_quick_sort(temp, 0, temp.size() - 1);
    }
}
BENCHMARK(BM_QuickSort)->Range(8, 131072);

// Benchmark Parallel QuickSort
static void BM_ParallelQuickSort(benchmark::State& state) {
    std::vector<int> arr = mt_generate_random_vector(state.range(0));
    for (auto _ : state) {
        std::vector<int> temp = arr;  // Copy before sorting
        mt_parallel_quick_sort(temp, 0, temp.size() - 1);
    }
}
BENCHMARK(BM_ParallelQuickSort)->Range(8, 131072);
