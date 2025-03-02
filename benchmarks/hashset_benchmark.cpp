#include <benchmark/benchmark.h>
#include "core/hashset.h"

#include <vector>
#include <unordered_map>
#include <chrono>
#include <unordered_set>

using namespace soul;

// Benchmark for HashSet_t
static void BM_HashSetInsert(benchmark::State& state) {
    HashSet_t<int> set;
    for (auto _ : state) {
        set.insert(state.range(0));
    }
}
BENCHMARK(BM_HashSetInsert)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_HashSetSearch(benchmark::State& state) {
    HashSet_t<int> set;
    for (int i = 0; i < state.range(0); ++i) {
        set.insert(i);
    }

    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            set.search(i);
        }
    }
}
BENCHMARK(BM_HashSetSearch)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_HashSetRemove(benchmark::State& state) {
    HashSet_t<int> set;
    for (int i = 0; i < state.range(0); ++i) {
        set.insert(i);
    }

    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            set.remove(i);
        }
    }
}
BENCHMARK(BM_HashSetRemove)->Arg(1000)->Arg(10000)->Arg(100000);

// Benchmark for std::vector
static void BM_VectorInsert(benchmark::State& state) {
    std::vector<int> vec;
    for (auto _ : state) {
        vec.push_back(state.range(0));
    }
}
BENCHMARK(BM_VectorInsert)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_VectorSearch(benchmark::State& state) {
    std::vector<int> vec(state.range(0));
    for (int i = 0; i < state.range(0); ++i) {
        vec[i] = i;
    }

    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            auto it = std::find(vec.begin(), vec.end(), i);
        }
    }
}
BENCHMARK(BM_VectorSearch)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_VectorRemove(benchmark::State& state) {
    std::vector<int> vec(state.range(0));
    for (int i = 0; i < state.range(0); ++i) {
        vec[i] = i;
    }

    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            vec.erase(std::remove(vec.begin(), vec.end(), i), vec.end());
        }
    }
}
BENCHMARK(BM_VectorRemove)->Arg(1000)->Arg(10000)->Arg(100000);

// Benchmark for std::unordered_map
static void BM_UnorderedMapInsert(benchmark::State& state) {
    std::unordered_map<int, int> map;
    for (auto _ : state) {
        map[state.range(0)] = state.range(0);
    }
}
BENCHMARK(BM_UnorderedMapInsert)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_UnorderedMapSearch(benchmark::State& state) {
    std::unordered_map<int, int> map;
    for (int i = 0; i < state.range(0); ++i) {
        map[i] = i;
    }

    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            auto it = map.find(i);
        }
    }
}
BENCHMARK(BM_UnorderedMapSearch)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_UnorderedMapRemove(benchmark::State& state) {
    std::unordered_map<int, int> map;
    for (int i = 0; i < state.range(0); ++i) {
        map[i] = i;
    }

    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            map.erase(i);
        }
    }
}
BENCHMARK(BM_UnorderedMapRemove)->Arg(1000)->Arg(10000)->Arg(100000);


// V2
// Google Benchmark for HashSet_t
static void BM_HashSetInsert2(benchmark::State& state) {
    HashSet_t<int> hashSet;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            hashSet.insert(i);
        }
    }
}
BENCHMARK(BM_HashSetInsert2)->Range(1<<10, 1<<20);

static void BM_HashSetSearch2(benchmark::State& state) {
    HashSet_t<int> hashSet;
    for (int i = 0; i < state.range(0); ++i) {
        hashSet.insert(i);
    }
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            benchmark::DoNotOptimize(hashSet.search(i));
        }
    }
}
BENCHMARK(BM_HashSetSearch2)->Range(1<<10, 1<<20);

// Google Benchmark for std::unordered_set
static void BM_UnorderedSetInsert2(benchmark::State& state) {
    std::unordered_set<int> stdSet;
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            stdSet.insert(i);
        }
    }
}
BENCHMARK(BM_UnorderedSetInsert2)->Range(1<<10, 1<<20);

static void BM_UnorderedSetSearch2(benchmark::State& state) {
    std::unordered_set<int> stdSet;
    for (int i = 0; i < state.range(0); ++i) {
        stdSet.insert(i);
    }
    for (auto _ : state) {
        for (int i = 0; i < state.range(0); ++i) {
            benchmark::DoNotOptimize(stdSet.find(i));
        }
    }
}
BENCHMARK(BM_UnorderedSetSearch2)->Range(1<<10, 1<<20);
