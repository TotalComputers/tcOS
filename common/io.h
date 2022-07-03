#pragma once

#include "frame_provider.h"
#include "input_handler.h"
#include <string>

class IOInterface : public FrameProvider, InputHandler {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;

public:
    short id;
    std::string name;

};