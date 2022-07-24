#pragma once

#include "internal/window.h"
#include "surface.h"
#include "../common/graphics.h"

class PBOSurface : public ISurface {
public:
    explicit PBOSurface(GLWindow*);

public:
    void render(IRenderer*) override;

public:
    image_t buffer {};

private:
    GLWindow* window = nullptr;
    unsigned int pbo[2] {};

    int dma = 0;
    int read = 1;

};