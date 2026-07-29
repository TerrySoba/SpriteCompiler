#pragma once

#include <stdint.h>
#include <vector>

enum RleType
{
    RLE_TYPE_CONSECUTIVE = 0 << 6, // memcpy
    RLE_TYPE_REPEAT = 1 << 6,      // memset
    RLE_TYPE_SKIP = 2 << 6,        // advance pointer, if count is 0, then skip to the end of the row
    RLE_TYPE_END = 3 << 6          // end of sprite data, always the last chunk
};


struct RLEChunk
{
    RleType type;
    uint8_t count;
    std::vector<uint8_t> data;
};

std::vector<RLEChunk> compressRLE(const std::vector<uint8_t>& input, uint16_t width, uint16_t height, uint8_t transparentColor);
std::vector<uint8_t> encodeRLE(const std::vector<RLEChunk>& chunks);