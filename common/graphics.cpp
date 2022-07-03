#include "graphics.h"
#include <malloc.h>

c_image_t image_t::as_c_image() {
    c_image_t dst;
    dst.data = data.data();
    dst.width = width;
    dst.height = height;
    return dst;
}

image_raw32_t c_image_t::as_raw_image() {
    image_raw32_t raw;
    raw.data = (uint32_t*)data;
    raw.width = width;
    raw.height = height;
    return raw;
}

image_t c_image_t::as_image() {
    image_t dst;
    dst.data = std::vector<color_argb_t>(data, data + width*height);
    dst.width = width;
    dst.height = height;
    return dst;   
}

image_raw8_t image_raw32_t::as_raw_image() {
    image_raw8_t raw;
    raw.data = (uint8_t*)data;
    raw.width = width;
    raw.height = height;
    raw.channels = 4;
    return raw;
}

c_image_t image_raw32_t::as_c_image() {
    c_image_t dst;
    dst.data = (color_argb_t*)data;
    dst.width = width;
    dst.height = height;
    return dst;
}

image_raw32_t image_raw8_t::as_uint_image() {
    image_raw32_t dst;
    if(channels == 4) {
        dst.data = (uint32_t*)data;
    } else {
        dst.data = (uint32_t*)calloc(width*height, 4);
        for(int i = 0; i < width*height; i++) {
            int base = i * channels;
            for(int c = 0; c < channels && c < 4; c++) {
                dst.data[i] <<= 8;
                dst.data[i] |= data[base + c];
            }
        }
    }
    dst.width = width;
    dst.height = height;
    return dst;
}

image_t new_empty_image(int width, int height) {
    image_t dst;
    dst.data.resize(width*height);
    dst.width = width;
    dst.height = height;
    return dst;
}