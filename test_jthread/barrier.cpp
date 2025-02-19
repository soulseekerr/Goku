
#include "main.h"

#include <iostream>
#include <barrier>
#include <thread>
#include <vector>
#include <chrono>
#include <random>

void worker_barrier(int id, std::barrier<>& barrier) {
    std::cout << "  # Thread " << id << " doing work before barrier\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));

    barrier.arrive_and_wait();  // All threads must reach this point before continuing

    std::cout << "  # Thread " << id << " passed the barrier\n";
}

void main_barrier() {
    constexpr int num_threads = 3;
    std::barrier barrier(num_threads);  // Requires num_threads to synchronize

    std::vector<std::jthread> jthreads;
    for (int i = 0; i < num_threads; ++i) {
        jthreads.emplace_back(worker_barrier, i + 1, std::ref(barrier));
    }

    std::cout << "  # All threads completed!" << std::endl;
}

