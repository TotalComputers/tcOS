#include "surface.h"

void NativeSurface::render(IRenderer* renderer) {
    renderer->render();
}

NativeSurface::~NativeSurface() = default;
