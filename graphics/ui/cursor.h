#pragma once

#include "element.h"
#include "../../input/input_handler.h"

class CursorElement : public CachedElement, public IInputHandler {
public:
    CursorElement(PositionedElement*, Shader*);

public:
    void thisChanged() override;
    void render(int layer) override;
    void display() override;
    void handleTouch(int, int, bool, bool) override;
    void handleMove(int, int) override;

private:
    Shader* cursorShader;
    double lastActive = 0;
    double lastClicked = 0;
    float opacity = 1;
    float state = 0;

};