#include "timer.h"
#include <thread>
#include <utility>

RepeatingTask::RepeatingTask() : active(false) {}

void RepeatingTask::start(const std::function<void(void)>& fn, long long delay, long long interval) {
    active = true;
    std::thread([this, fn, delay, interval]() {
        if (!active) {
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if (!active) {
            return;
        }
        while (true) {
            fn();
            if(!active) {
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(!active) {
                return;
            }
        }
    }).detach();
}

void RepeatingTask::start(const std::function<void(void)>& fn, long long interval) {
    start(fn, 0, interval);
}

void RepeatingTask::stop() {
    active = false;
}

DelayedTask::DelayedTask() = default;

void DelayedTask::start(const std::function<void(void)>& fn, long long delay) {
    std::thread([fn, delay]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        fn();
    }).detach();
}

void MultiThreadRepeatingTask::start(const std::function<void(void)>& fn, long long delay, long long interval) {
    active = true;
    std::thread([this, fn, delay, interval]() {
        if (!active) {
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if (!active) {
            return;
        }
        while(true) {
            std::thread(fn).detach();
            if (!active) {
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if (!active) {
                return;
            }
        }
    }).detach();
}

void MultiThreadRepeatingTask::start(const std::function<void(void)>& fn, long long interval) {
    start(fn, 0, interval);
}