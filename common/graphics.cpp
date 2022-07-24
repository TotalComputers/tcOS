#include "graphics.h"
#include <malloc.h>

image_t new_empty_image(int width, int height) {
    image_t dst {};
    dst.data = (color_argb_t*) calloc(width*height, sizeof(color_argb_t));
    dst.width = width;
    dst.height = height;
    return dst;
}