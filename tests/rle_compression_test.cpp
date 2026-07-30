#include <catch2/catch_all.hpp>

#include "tga_image.h"
#include "rle_compression.h"

TEST_CASE("Zero-count skip chunk is emitted for transparent runs to the row end", "[rle_compression]")
{
    std::vector<uint8_t> inputData = { 0x01, 0x00, 0x00, 0x00 };
    std::vector<RLEChunk> compressedData = compressRLE(inputData, 4, 1, 0x00);

    REQUIRE(compressedData.size() == 3);
    REQUIRE(compressedData[0].type == RLE_TYPE_CONSECUTIVE);
    REQUIRE(compressedData[0].count == 1);
    REQUIRE(compressedData[1].type == RLE_TYPE_SKIP);
    REQUIRE(compressedData[1].count == 0);
    REQUIRE(compressedData[1].data.empty());
    REQUIRE(compressedData[2].type == RLE_TYPE_END);
}

TEST_CASE("Each row ends with a zero-count skip chunk", "[rle_compression]")
{
    std::vector<uint8_t> inputData = {
        0x01, 0x02,
        0x03, 0x00
    };

    std::vector<RLEChunk> compressedData = compressRLE(inputData, 2, 2, 0x00);

    REQUIRE(compressedData.size() == 5);

    REQUIRE(compressedData[0].type == RLE_TYPE_CONSECUTIVE);
    REQUIRE(compressedData[0].count == 2);
    REQUIRE(compressedData[0].data == std::vector<uint8_t>({ 0x01, 0x02 }));

    REQUIRE(compressedData[1].type == RLE_TYPE_SKIP);
    REQUIRE(compressedData[1].count == 0);

    REQUIRE(compressedData[2].type == RLE_TYPE_CONSECUTIVE);
    REQUIRE(compressedData[2].count == 1);
    REQUIRE(compressedData[2].data == std::vector<uint8_t>({ 0x03 }));

    REQUIRE(compressedData[3].type == RLE_TYPE_SKIP);
    REQUIRE(compressedData[3].count == 0);

    REQUIRE(compressedData[4].type == RLE_TYPE_END);
}

TEST_CASE("Good Case", "[rle_compression]")
{
    std::string spriteFilePath = std::string(TEST_DATA_DIR) + std::string("/") +
        "guyframe.tga"; // GENERATE("porframe.tga", "guyframe.tga");

    TgaImage img(spriteFilePath.c_str());
    // REQUIRE(img.width() == 123);
    // REQUIRE(img.height() == 123);   

    std::vector<uint8_t> inputData(img.data(), img.data() + (img.width() * img.height()));
    uint8_t transparentColor = 0; // Replace with the actual transparent color if needed
    std::vector<RLEChunk> compressedData = compressRLE(inputData, img.width(), img.height(), transparentColor);

    REQUIRE(!compressedData.empty());

    // dump the compressed data for inspection
    for (const auto& chunk : compressedData)
    {
        // Print chunk information for debugging and convert enum to string representation
        const char* typeStr = "";
        switch (chunk.type)
        {
            case RLE_TYPE_CONSECUTIVE: typeStr = "CONSECUTIVE"; break;
            case RLE_TYPE_REPEAT: typeStr = "REPEAT"; break;
            case RLE_TYPE_SKIP: typeStr = "SKIP"; break;
            case RLE_TYPE_END: typeStr = "END"; break;
        }
        printf("Chunk Type: %s, Count: %d, Data Size: %zu\n", typeStr, chunk.count, chunk.data.size());
    } 

    printf("Image size: %d x %d, Compressed chunks: %zu\n", img.width(), img.height(), compressedData.size());



    auto encodedData = encodeRLE(compressedData);
    printf("Encoded data size: %zu\n", encodedData.size());

}


TEST_CASE("Another Case", "[rle_compression]")
{
    std::vector<uint8_t> inputData = 
        { 0x00, 0x00, 0x01,
          0x02, 0x02, 0x02,
          0x00, 0x00, 0x00,
          0x03, 0x04, 0x00 };

    int width = 3;
    int height = inputData.size() / width;

    uint8_t transparentColor = 0x00; // Assuming 0x00 is the transparent color
    std::vector<RLEChunk> compressedData = compressRLE(inputData, width, height, transparentColor);

    REQUIRE(compressedData.size() > 0);
    REQUIRE(compressedData.back().type == RLE_TYPE_END);

    REQUIRE(compressedData[0].type == RLE_TYPE_SKIP);
    REQUIRE(compressedData[0].count == 2);
    REQUIRE(compressedData[0].data.empty());

    REQUIRE(compressedData[1].type == RLE_TYPE_CONSECUTIVE);
    REQUIRE(compressedData[1].count == 1);
    REQUIRE(compressedData[1].data.size() == 1);
    REQUIRE(compressedData[1].data[0] == 0x01);

    REQUIRE(compressedData[2].type == RLE_TYPE_SKIP); // lineskip
    REQUIRE(compressedData[2].count == 0);
    REQUIRE(compressedData[2].data.empty());

    REQUIRE(compressedData[3].type == RLE_TYPE_REPEAT);
    REQUIRE(compressedData[3].count == 3);
    REQUIRE(compressedData[3].data.size() == 1);
    REQUIRE(compressedData[3].data[0] == 0x02);

    REQUIRE(compressedData[4].type == RLE_TYPE_SKIP);
    REQUIRE(compressedData[4].count == 0);
    REQUIRE(compressedData[4].data.empty());

    REQUIRE(compressedData[5].type == RLE_TYPE_SKIP);
    REQUIRE(compressedData[5].count == 0);
    REQUIRE(compressedData[5].data.empty());

    REQUIRE(compressedData[6].type == RLE_TYPE_CONSECUTIVE);
    REQUIRE(compressedData[6].count == 2);
    REQUIRE(compressedData[6].data == std::vector<uint8_t>({ 0x03, 0x04 }));

    REQUIRE(compressedData[7].type == RLE_TYPE_SKIP);
    REQUIRE(compressedData[7].count == 0);
    REQUIRE(compressedData[7].data.empty());

    REQUIRE(compressedData[8].type == RLE_TYPE_END);



}