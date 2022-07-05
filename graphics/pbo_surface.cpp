#include <glad/glad.h>
#include "pbo_surface.h"
#include <cstring>

PBOSurface::PBOSurface(GLWindow* window) 
    : window(window) {
    glGenBuffers(2, pbo);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER, window->getWidth() * window->getHeight() * 4, 0, GL_STREAM_READ);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo[1]);
    glBufferData(GL_PIXEL_PACK_BUFFER, window->getWidth() * window->getHeight() * 4, 0, GL_STREAM_READ);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    buffer = new_empty_image(window->getWidth(), window->getHeight());
}

void PBOSurface::render(IRenderer* renderer) {
    renderer->render();

    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo[dma]);
    glReadPixels(0, 0, window->getWidth(), window->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo[read]);
    unsigned char* data = (unsigned char*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    unsigned scanline = 4 * window->getWidth();

    for(int y = 0; y < window->getHeight(); y++) {
        memcpy(buffer.raw8 + y * scanline, data + (window->getHeight() - 1 - y) * scanline, scanline);
    }

    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    std::swap(read, dma);
}