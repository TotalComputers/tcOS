#pragma once

#include "internal/window.h"
#include "surface.h"
#include "../common/graphics.h"

class PBOSurface : public ISurface {
public:
    PBOSurface(GLWindow*);

public:
    void render(IRenderer*) override;

public:
    image_t buffer;

private:
    GLWindow* window;
    unsigned int pbo[2];

    int dma = 0;
    int read = 1;

};