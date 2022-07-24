#include "thread_safety.h"
#include <list>
#include <mutex>

static std::list<std::function<void(void)>> tasks;
static std::mutex mutex;

void run_in_main_thread(std::function<void(void)> fn) {
    bool done = false;
    mutex.lock();
    tasks.emplace_back([&]() {
        fn();
        done = true;
    });
    mutex.unlock();
    while(!done);
}

[[noreturn]] void run_loop() {
    while (true) {
        if (!tasks.empty()) {
            mutex.lock();
            std::list<std::function<void(void)>> copy = tasks;
            for (auto& fn : copy) {
                fn();
                tasks.pop_front();
            }
            mutex.unlock();
        }
    }
}