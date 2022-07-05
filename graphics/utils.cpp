#include <glad/glad.h>
#include "utils.h"

void GLUtils::enableTransparency() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLUtils::disableTransparency() {
    glDisable(GL_BLEND);
}