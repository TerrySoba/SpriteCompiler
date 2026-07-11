#include <catch2/catch_test_macros.hpp>

#include "compiled_sprite.h"

void writeVectorToDisk(const std::vector<char>& data, const std::string& filename)
{
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    size_t bytesWritten = fwrite(data.data(), 1, data.size(), file);
    if (bytesWritten != data.size()) {
        fclose(file);
        throw std::runtime_error("Failed to write all data to file: " + filename);
    }

    fclose(file);
}

std::vector<char> readVectorFromDisk(const std::string& filename)
{
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::vector<char> data(fileSize);
    size_t bytesRead = fread(data.data(), 1, fileSize, file);
    if (bytesRead != static_cast<size_t>(fileSize)) {
        fclose(file);
        throw std::runtime_error("Failed to read all data from file: " + filename);
    }

    fclose(file);
    return data;
}

void blit(const std::vector<char>& source, int sourceWidth, int sourceHeight,
          std::vector<char>& target, int targetWidth, int targetHeight,
          int posX, int posY)
{
    for (int y = 0; y < sourceHeight; ++y)
    {
        for (int x = 0; x < sourceWidth; ++x)
        {
            int targetX = posX + x;
            int targetY = posY + y;

            if (targetX >= 0 && targetX < targetWidth && targetY >= 0 && targetY < targetHeight)
            {
                target[targetY * targetWidth + targetX] = source[y * sourceWidth + x];
            }
        }
    }
}

void deleteFileIfExists(const std::string& filename)
{
    if (std::remove(filename.c_str()) != 0)
    {
        // File might not exist, ignore the error
    }
}

TEST_CASE("sprite names are normalized to uppercase", "[sprite_compiler]")
{
    int bufferWidth = 320;
    int bufferHeight = 200;
    int targetPosX = 10;
    int targetPosY = 10;

    printf("Testdata dir: %s\n", TEST_DATA_DIR);

    CompiledSprite sprite(TEST_DATA_DIR "/guyframe.tga", 320);
    REQUIRE(sprite.width() > 0);
    REQUIRE(sprite.height() > 0);
    
    const std::vector<char>& compiledFunction = sprite.getCompiledFunction();
    REQUIRE(!compiledFunction.empty());

    writeVectorToDisk(compiledFunction, "output.bin");

    deleteFileIfExists("BUFFER.BIN");

    // This command executes the compiled sprite in a DOS environment and writes the output to buffer.bin
    system(PROJECT_BASE_DIR "/run_dos_test.sh");

    // Now read buffer and check if compiled sprite executed correctly
    std::vector<char> outputData = readVectorFromDisk("BUFFER.BIN");

    // recreate the original image data from the sprite
    std::vector<char> buffer(bufferWidth * bufferHeight);
    std::fill(buffer.begin(), buffer.end(), 0); // Clear the buffer
    blit(sprite.getOriginalData(), sprite.width(), sprite.height(), buffer, bufferWidth, bufferHeight, targetPosX, targetPosY);

    // Compare the output data with the expected buffer
    REQUIRE(outputData.size() == buffer.size());
    for (size_t x = 0; x < bufferHeight; ++x) {
        for (size_t y = 0; y < bufferWidth; ++y) {
            size_t i = x * bufferWidth + y;
            INFO("Comparing pixel at (" << y << ", " << x << "): outputData=" << (int)(unsigned char)outputData[i] << ", buffer=" << (int)(unsigned char)buffer[i]);
            REQUIRE(outputData[i] == buffer[i]);
        }
    }
}