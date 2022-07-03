#pragma once

#include <cstdint>
#include <vector>

void set_palette(const uint32_t*, int);
void cache_palette();
unsigned char match_color(uint32_t);
std::vector<unsigned char> match_image(const uint8_t*, int, int);
std::vector<unsigned char> match_image(const uint8_t*, int, int, int);