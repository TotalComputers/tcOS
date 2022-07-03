#pragma once

class InputHandler {
public:
    virtual void handle_touch(int, int, bool, bool) = 0;

};