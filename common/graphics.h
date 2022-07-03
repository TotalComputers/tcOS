#pragma once

#include <cstdint>
#include <vector>

struct image_t;
struct c_image_t;
struct image_raw32_t;
struct image_raw8_t;

union color_argb_t {
    struct {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };
    uint32_t argb;
};

struct image_t {
    std::vector<color_argb_t> data;
    uint32_t width;
    uint32_t height;

    c_image_t as_c_image();
};

struct c_image_t {
    color_argb_t* data;
    uint32_t width;
    uint32_t height;

    image_raw32_t as_raw_image();
    image_t as_image();
};

struct image_raw32_t {
    uint32_t* data;
    uint32_t width;
    uint32_t height;

    image_raw8_t as_raw_image();
    c_image_t as_c_image();
};

struct image_raw8_t {
    uint8_t* data;
    uint32_t width;
    uint32_t height;
    uint8_t channels;

    image_raw32_t as_uint_image();
};

image_t new_empty_image(int, int);