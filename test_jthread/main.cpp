
#include "main.h"


int main() {
    // std::jthread
    main_jthread();
    // latch std::thread
    main_latch();
    // latch std::jthread
    main_latch_jthread();
    // latch std::jthread with cancel
    main_latch_jthread_cancel();
    // latch std::jthread with cancel, with shared_ptr
    main_latch_jthread_cancel_with_shared_ptr();
    // latch std::jthread with cancel, with unique_ptr
    main_latch_jthread_cancel_with_unique_ptr();
    // latch std::jthread with cancel, with lambda
    main_latch_jthread_cancel_with_lambda();

    // barrier std::jthread
    main_barrier();

    return 0;
}