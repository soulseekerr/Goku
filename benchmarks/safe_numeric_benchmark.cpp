#include <benchmark/benchmark.h>
#include <thread>
#include <vector>
#include "core.h"  // SafeNumeric<>

// Benchmark SafeNumeric Single Thread
static void BM_SafeNumeric_Increment(benchmark::State& state) {
    SafeNumeric<int> num(0);
    for (auto _ : state) {
        num.increment();
    }
}
BENCHMARK(BM_SafeNumeric_Increment);

static void BM_SafeNumeric_Add(benchmark::State& state) {
    SafeNumeric<int> num(0);
    for (auto _ : state) {
        num.add(10);
    }
}
BENCHMARK(BM_SafeNumeric_Add);

// Benchmark SafeNumeric Multi-Threaded
static void BM_SafeNumeric_Increment_MultiThreaded(benchmark::State& state) {
    SafeNumeric<int> num(0);
    constexpr int THREADS = 10;
    
    for (auto _ : state) {
        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; ++i) {
            threads.emplace_back([&]() {
                for (int j = 0; j < 1000; ++j) {
                    num.increment();
                }
            });
        }
        for (auto& t : threads) t.join();
    }
}
BENCHMARK(BM_SafeNumeric_Increment_MultiThreaded)->Threads(10);

// Benchmark SafeFlag
static void BM_SafeFlag_Set(benchmark::State& state) {
    SafeFlag flag;
    for (auto _ : state) {
        flag.set();
    }
}
BENCHMARK(BM_SafeFlag_Set);

static void BM_SafeFlag_Clear(benchmark::State& state) {
    SafeFlag flag;
    for (auto _ : state) {
        flag.clear();
    }
}
BENCHMARK(BM_SafeFlag_Clear);

// Multi-threaded SafeFlag
static void BM_SafeFlag_MultiThreaded(benchmark::State& state) {
    SafeFlag flag;
    constexpr int THREADS = 10;

    for (auto _ : state) {
        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; ++i) {
            threads.emplace_back([&]() {
                flag.set();
                flag.clear();
            });
        }
        for (auto& t : threads) t.join();
    }
}
BENCHMARK(BM_SafeFlag_MultiThreaded)->Threads(10);

BENCHMARK_MAIN();
