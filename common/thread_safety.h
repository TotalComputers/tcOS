#pragma once
#include <functional>

void run_in_main_thread(std::function<void(void)>);
void run_loop();