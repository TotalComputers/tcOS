#include "surface.h"

int CombinedSurface::addSurface(ISurface* surface) {
    surfaces.push_back(surface);
    return surfaces.size() - 1;
}

ISurface* CombinedSurface::getSurface(int id) {
    return surfaces[id];
}

void CombinedSurface::render(IRenderer* renderer) {
    for(ISurface*& surface : surfaces)
        surface->render(renderer);
}