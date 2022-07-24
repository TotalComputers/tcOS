#pragma once

#include <functional>

class RepeatingTask {
public:
    RepeatingTask();
    virtual void start(const std::function<void(void)>&, long long, long long);
    virtual void start(const std::function<void(void)>&, long long);
    virtual void stop();

protected:
    bool active;

};

class DelayedTask {
public: 
    DelayedTask();
    static void start(const std::function<void(void)>&, long long);

};

class MultiThreadRepeatingTask : public RepeatingTask {
public:
    void start(const std::function<void(void)>&, long long, long long) override;
    void start(const std::function<void(void)>&, long long) override;

};