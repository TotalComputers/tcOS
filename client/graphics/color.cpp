#include "color.h"

std::vector<uint32_t> colors;
std::vector<uint8_t> cache;

static double get_distance(uint8_t c1r, uint8_t c1g, uint8_t c1b, uint32_t c2) {
    uint8_t c2r = (c2 & 0x00FF0000) >> 16;
    uint8_t c2g = (c2 & 0x0000FF00) >> 8;
    uint8_t c2b = (c2 & 0x000000FF);

    double rMean = (double) (c1r + c2r) / 2.0;
    double r = c1r - c2r;
    double g = c1g - c2g;
    int b = c1b - c2b;
    double weightR = 2.0 + rMean / 256.0;
    double weightG = 4.0;
    double weightB = 2.0 + (255.0 - rMean) / 256.0;
    return weightR * r * r + weightG * g * g + weightB * (double) b * (double) b;
}

static unsigned char match_color_slow(uint8_t r, uint8_t g, uint8_t b) {
    int idx = 0;
    double best = -1.0;

    for (int i = 4; i < colors.size(); ++i) {
        double distance = get_distance(r, g, b, colors[i]);
        if (distance < best || best == -1.0) {
            best = distance;
            idx = i;
        }
    }

    return (unsigned char) (idx < 128? idx : -129 + (idx - 127));
}


void set_palette(const uint32_t* palette, int len) {
    colors = std::vector<uint32_t>(palette, palette + len);
}

void cache_palette() {
    cache.resize(0x1000);
    for (int i = 0; i <= 0xFFF; i++) {
        cache[i] = match_color_slow((i & 0xF00) >> 4, i & 0xF0, (i & 0xF) << 4);
    }
}

unsigned char match_color(uint32_t rgb) {
    return cache[(rgb & 0xF00000) >> 12 | (rgb & 0xF000) >> 8 | (rgb & 0xF0) >> 4];
}

std::vector<unsigned char> match_image(const uint8_t* data, int len, int channels) {
    std::vector<unsigned char> dst;
    dst.resize(len);
    for (int i = 0; i < len; i++) {
        int base = i * channels;
        dst[i] = match_color(((uint32_t) data[base] << 16) | ((uint32_t) data[base + 1] << 8) | data[base + 2]);
    }
    return dst;
}

std::vector<unsigned char> match_image(const uint8_t* data, int width, int height, int channels) {
    return match_image(data, width * height, channels);
}