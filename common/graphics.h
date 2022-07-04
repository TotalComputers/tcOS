#pragma once

#include <cstdint>
#include <vector>

union color_argb_t {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
    uint32_t argb;
};

struct image_t {
    union {
        color_argb_t* data;
        uint32_t* raw32;
        uint8_t* raw8;
    };
    int width;
    int height;
};

image_t new_empty_image(int, int);