
#include "main.h"


/**
 * Simple example with jthread.
 */

void worker_jthread() {
    for (int i = 0; i < 5; i++) {
        std::cout << "  # Worker thread running: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void main_jthread() {
    std::cout << "> Calling jthread" << std::endl;
    // RAII (Resource Acquisition Is Initialization) thread
    std::jthread jt(worker_jthread);  // start a thread that automatically joins on exit.

    std::cout << "  # Main thread continues...\n";
    // No need to call jt.join(), it will auto-join when going out of scope.
}

/**
 * Simple example with latch.
 */

void worker_latch(std::latch& latch, int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * id));
    std::cout << "  # Thread " << id << " finished work\n";
    latch.count_down();  // Decrease latch counter
}

void main_latch() {
    std::cout << "> Calling latch with thread" << std::endl;

    constexpr int num_threads = 5;
    std::latch latch(num_threads);  // Initialize with thread count

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker_latch, std::ref(latch), i + 1);
    }

    latch.wait();  // Wait until all workers call count_down()
    std::cout << "  # All threads finished, proceeding with main thread\n";

    for (auto& t : threads) {
        t.join();
    }
}

void main_latch_jthread() {
    std::cout << "> Calling latch with jthread" << std::endl;

    constexpr int num_threads = 5;
    std::latch latch(num_threads);  // Initialize with thread count

    std::vector<std::jthread> jthreads;
    for (int i = 0; i < num_threads; ++i) {
        jthreads.emplace_back(worker_latch, std::ref(latch), i + 1);
    }

    latch.wait();  // Wait until all workers call count_down()
    std::cout << "  # All threads finished, proceeding with main thread\n";

    // No need to call jt.join(), it will auto-join when going out of scope.
}

// Simulated workload with a possibility of early cancellation
void worker_latch_cancel(std::stop_token stop_token, std::latch& latch, int id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 500); // Random delay

    int work_time = dist(gen);
    std::cout << "  # Thread " << id << " starting, will work for " << work_time << " ms\n";

    for (int i = 0; i < work_time / 50; ++i) {
        if (stop_token.stop_requested()) {
            std::cout << "  # Thread " << id << " received stop request, exiting early\n";
            latch.count_down();
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "Thread " << id << " finished work\n";
    latch.count_down();
}

void main_latch_jthread_cancel() {
    std::cout << "> Calling latch with jthread and cancel action" << std::endl;

    constexpr int num_threads = 5;
    std::latch latch(num_threads);
    std::vector<std::jthread> threads;
    std::stop_source stop_source;

    // Launch worker threads
    for (int i = 0; i < num_threads; ++i) {
        
        /** 
         * 1) std::latch is non-copyable: if we tried to pass latch directly, 
         *    jthread would attempt to copy it, which would cause a compilation error.
         * 
         * 2) avoid unnecessary heap allocations: without std::ref, we'd had to wrap latch 
         *    in a std::shared_ptr or similar to share it between threads.
         * 
         * 3) ensures shared synchronization: all threads must operate on the std::latch instance to
         *    properly decrement its counter and allow synchronization.
         * 
         * 4) std::reference_wrapper<std::latch>  <=> std::ref()
         */

        threads.emplace_back(worker_latch_cancel, stop_source.get_token(), std::ref(latch), i + 1);
    }

    // Simulate an early termination condition (e.g., user aborts)
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "  # Main thread: Cancelling remaining work\n";
    stop_source.request_stop(); // Notify workers to stop

    // Wait for all workers to finish
    latch.wait();
    std::cout << "> All threads completed or were stopped. Main thread proceeding.\n";
}

void worker_latch_cancel_with_shared_ptr(std::stop_token stop_token, std::shared_ptr<std::latch> latch, int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));

    if (stop_token.stop_requested()) {
        std::cout << "Thread " << id << " received stop request, exiting early\n";
        latch->count_down();
        return;
    }

    std::cout << "Thread " << id << " finished work\n";
    latch->count_down();
}

void main_latch_jthread_cancel_with_shared_ptr() {
    std::cout << "> Calling latch with jthread and cancel action, with shared_ptr" << std::endl;

    constexpr int num_threads = 5;
    auto latch = std::make_shared<std::latch>(num_threads); // Use a shared_ptr for ownership

    std::vector<std::jthread> threads;
    std::stop_source stop_source;

    // Launch worker threads
    for (int i = 0; i < num_threads; ++i) {
        
        /** 
         * 1) std::latch is non-copyable: instead of using std::ref, we use shared_ptr.
         *    All threads share ownership of the latch via std::shared_ptr.
         * 
         * 2) Safer Lifetime management: if a thread outlives the scope of the function, 
         *    shared_ptr ensures the std::latch remains valid.
         * 
         * 3) Allows Dynamic Allocation: std::shared_ptr is a clean way 
         *    to manage the dynamic allocation of the latch.
         */

        threads.emplace_back(worker_latch_cancel_with_shared_ptr, stop_source.get_token(), latch, i + 1);
    }

    // Simulate an early termination condition (e.g., user aborts)
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "  # Main thread: Cancelling remaining work\n";
    stop_source.request_stop(); // Notify workers to stop

    // Wait for all workers to finish
    latch->wait();
    std::cout << "> All threads completed or were stopped. Main thread proceeding.\n";
}

void worker_latch_cancel_with_unique_ptr(std::stop_token stop_token, std::unique_ptr<std::latch>& latch, int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));

    if (stop_token.stop_requested()) {
        std::cout << "Thread " << id << " received stop request, exiting early\n";
        latch->count_down();
        return;
    }

    std::cout << "Thread " << id << " finished work\n";
    latch->count_down();
}


void main_latch_jthread_cancel_with_unique_ptr() {
    std::cout << "> Calling latch with jthread and cancel action, with unique_ptr" << std::endl;

    constexpr int num_threads = 5;
    auto latch = std::make_unique<std::latch>(num_threads); // Unique ownership

    std::vector<std::jthread> threads;
    std::stop_source stop_source;

    // Launch worker threads
    for (int i = 0; i < num_threads; ++i) {
        
        /** 
         * 1) Using std::unique_ptr avoids unnessariy reference passing std::ref.
         * 2) Ensures only one owner.
         * 3) More efficient than std::shared_ptr if we don't need shared ownership.
         * 4) we must pass std::unique_ptr<std::latch>& because std::unique_ptr 
         *    cannot be copied, only moved. 
         *    This means our worker function has to accept a reference to the unique pointer.
         */
        threads.emplace_back(worker_latch_cancel_with_unique_ptr, stop_source.get_token(), std::ref(latch), i + 1);
    }

    // Simulate an early termination condition (e.g., user aborts)
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "  # Main thread: Cancelling remaining work\n";
    stop_source.request_stop(); // Notify workers to stop

    // Wait for all workers to finish
    latch->wait();
    std::cout << "> All threads completed or were stopped. Main thread proceeding.\n";
}

void worker_latch_cancel_with_lambda(std::stop_token stop_token, std::latch& latch, int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));

    if (stop_token.stop_requested()) {
        std::cout << "Thread " << id << " received stop request, exiting early\n";
        latch.count_down();
        return;
    }

    std::cout << "Thread " << id << " finished work\n";
    latch.count_down();
}

void main_latch_jthread_cancel_with_lambda() {
    std::cout << "> Calling latch with jthread and cancel action, with lambda" << std::endl;

    constexpr int num_threads = 5;
    auto latch = std::make_unique<std::latch>(num_threads); // Unique ownership

    std::vector<std::jthread> threads;
    std::stop_source stop_source;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, id = i + 1]() {
            worker_latch_cancel_with_lambda(stop_source.get_token(), *latch, id);
        });
    }

    // Simulate an early termination condition (e.g., user aborts)
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "  # Main thread: Cancelling remaining work\n";
    stop_source.request_stop(); // Notify workers to stop

    // Wait for all workers to finish
    latch->wait();
    std::cout << "> All threads completed or were stopped. Main thread proceeding.\n";
}