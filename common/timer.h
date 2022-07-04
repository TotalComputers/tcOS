#pragma once

#include <functional>

class RepeatingTask {
public:
    RepeatingTask();
    virtual void start(std::function<void(void)>, long long, long long);
    virtual void start(std::function<void(void)>, long long);
    virtual void stop();

protected:
    bool active;

};

class DelayedTask {
public: 
    DelayedTask();
    void start(std::function<void(void)>, long long);
    void cancel();

private:
    bool canceled;

};

class MultiThreadRepeatingTask : public RepeatingTask {
public:
    void start(std::function<void(void)>, long long, long long) override;
    void start(std::function<void(void)>, long long) override;

};