#pragma once
#include <functional>

void run_in_main_thread(std::function<void(void)>);

[[noreturn]] void run_loop();