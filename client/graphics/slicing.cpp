#include "slicing.h"

std::vector<unsigned char> slice_indices(std::vector<unsigned char> raw, int width, int height) {
    std::vector<unsigned char> dst;
    dst.resize(raw.size());

    for (int y = 0; y < height / 128; y++) {
        for (int x = 0; x < width / 128; x++) {
            int idx = y * (width / 128) + x;
            for (int sx = 0; sx < 128; sx++) {
                for (int sy = 0; sy < 128; sy++) {
                    dst[idx * 128 * 128 + sy * 128 + sx] =
                        raw[(y * 128 + sy) * width + x * 128 + sx];
                }
            }
        } 
    }
    
    return dst;
}