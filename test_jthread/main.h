
#include <iostream>
#include <thread>
#include <chrono>
#include <latch>
#include <vector>
#include <stop_token>
#include <random>
#include <memory>
#include <barrier>


// std::jthread and std::latch

void worker_jthread();
void worker_latch(std::latch& latch, int id);
void worker_latch_cancel(std::stop_token stop_token, std::latch& latch, int id);
void worker_latch_cancel_with_shared_ptr(std::stop_token stop_token, std::shared_ptr<std::latch> latch, int id);
void worker_latch_cancel_with_unique_ptr(std::stop_token stop_token, std::unique_ptr<std::latch>& latch, int id);

void main_jthread();
void main_latch();
void main_latch_jthread();
void main_latch_jthread_cancel();
void main_latch_jthread_cancel_with_shared_ptr();
void main_latch_jthread_cancel_with_unique_ptr();
void main_latch_jthread_cancel_with_lambda();
void worker_latch_cancel_with_lambda(std::stop_token stop_token, std::latch& latch, int id);


// std::barrier

void main_barrier();
