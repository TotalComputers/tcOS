#include "thread_safety.h"
#include <list>
#include <mutex>

std::list<std::function<void(void)>> tasks;
std::mutex mutex;

void run_in_main_thread(std::function<void(void)> fn) {
    bool done = false;
    mutex.lock();
    tasks.push_back([&]() {
        fn();
        done = true;
    });
    mutex.unlock();
    while(!done);
}

void run_loop() {
    while(1) {
        if(tasks.size() > 0) {
            mutex.lock();
            std::list<std::function<void(void)>> copy = tasks;
            for(auto& fn : copy) {
                fn();
                tasks.pop_front();
            }
            mutex.unlock();
        }
    }
}