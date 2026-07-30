#include "rle_compression.h"

std::vector<RLEChunk> compressRLE(const std::vector<uint8_t>& input, uint16_t width, uint16_t height, uint8_t transparentColor)
{
    std::vector<RLEChunk> chunks;

    for(uint16_t y = 0; y < height; ++y)
    {
        const uint16_t rowStart = y * width;
        const uint16_t rowEnd = rowStart + width;
        const bool isLastRow = (y + 1 == height);

        for(uint16_t x = rowStart; x < rowEnd;)
        {
            const uint8_t currentColor = input[x];
            uint8_t count = 1;

            while(x + count < rowEnd && input[x + count] == currentColor && count < 255)
            {
                ++count;
            }

            RLEChunk chunk;
            if(currentColor == transparentColor)
            {
                if(isLastRow && x + count == rowEnd)
                {
                    x += count;
                    break;
                }

                chunk.type = RLE_TYPE_SKIP;
                chunk.count = (x + count == rowEnd) ? 0 : count;
            }
            else if(count > 1)
            {
                chunk.type = RLE_TYPE_REPEAT;
                chunk.count = count;
                chunk.data.push_back(currentColor);
            }
            else
            {
                while(x + count < rowEnd && input[x + count] != transparentColor && count < 63)
                {
                    const uint8_t nextColor = input[x + count];

                    if(x + count + 1 < rowEnd && input[x + count + 1] == nextColor)
                    {
                        break;
                    }

                    ++count;
                }

                chunk.type = RLE_TYPE_CONSECUTIVE;
                chunk.count = count;
                chunk.data.insert(chunk.data.end(), input.begin() + x, input.begin() + x + count);
            }

            chunks.push_back(chunk);
            x += count;
        }

        if(!isLastRow && (chunks.empty() || chunks.back().type != RLE_TYPE_SKIP || chunks.back().count != 0))
        {
            chunks.push_back({ RLE_TYPE_SKIP, 0, {} });
        }
    }

    RLEChunk endChunk;
    endChunk.type = RLE_TYPE_END;
    endChunk.count = 0;
    chunks.push_back(endChunk);

    return chunks;
}


std::vector<uint8_t> encodeRLE(const std::vector<RLEChunk>& chunks)
{
    std::vector<uint8_t> encodedData;

    for(const auto& chunk : chunks)
    {
        uint8_t typeAndCount = static_cast<uint8_t>(chunk.type) | (chunk.count & 0b00111111);
        encodedData.push_back(typeAndCount);

        if(chunk.type == RLE_TYPE_CONSECUTIVE || chunk.type == RLE_TYPE_REPEAT)
        {
            encodedData.insert(encodedData.end(), chunk.data.begin(), chunk.data.end());
        }
    }

    return encodedData;
}
