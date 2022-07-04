#pragma once

#include "window.h"

class IWindowFactory {
public:
    virtual GLWindow* createWindow(int, int, std::string) = 0;

};