#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <exception>

typedef void (*CompiledSpritePtr)(void* target, uint16_t targetWidth);


class CompiledSprite
{
public:
    CompiledSprite(const char* filename)
    {
        FILE* file = fopen(filename, "rb");
        if (!file) {
            throw std::runtime_error(std::string("Failed to open file: ") + filename);
        }
        
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        m_compiledSpriteCode.resize(fileSize);
        size_t bytesRead = fread(&m_compiledSpriteCode[0], 1, fileSize, file);
        if (bytesRead != static_cast<size_t>(fileSize)) {
            fclose(file);
            throw std::runtime_error(std::string("Failed to read file: ") + filename);
        }

        fclose(file);
    }

    size_t getCompiledCodeSize() const {
        return m_compiledSpriteCode.size();
    }

    void execute(void* target, uint16_t targetWidth) const {
        if (m_compiledSpriteCode.empty()) {
            throw std::runtime_error("Compiled sprite code is empty.");
        }

        CompiledSpritePtr func = (CompiledSpritePtr)(&m_compiledSpriteCode[0]);
        func(target, targetWidth);
    }


private:
    std::vector<uint8_t> m_compiledSpriteCode;

};


struct Parameters
{
    std::string outputFile;
    std::string compiledSpriteFile;
    uint16_t bufferWidth;
    uint16_t bufferHeight;
    uint16_t targetPosX;
    uint16_t targetPosY;
};


Parameters parseCommandLineArguments(int argc, char* argv[])
{
    Parameters params;

    if (argc < 7) {
        throw std::runtime_error("Usage: program <output_file> <compiled_sprite_file> <buffer_width> <buffer_height> <target_pos_x> <target_pos_y>");
    }

    params.outputFile = argv[1];
    params.compiledSpriteFile = argv[2];
    params.bufferWidth = static_cast<uint16_t>(atoi(argv[3]));
    params.bufferHeight = static_cast<uint16_t>(atoi(argv[4]));
    params.targetPosX = static_cast<uint16_t>(atoi(argv[5]));
    params.targetPosY = static_cast<uint16_t>(atoi(argv[6]));

    return params;
}

int main(int argc, char* argv[])
{
    try
    {
        Parameters params = parseCommandLineArguments(argc, argv);
        std::string outputFile = params.outputFile;
        std::string compiledSpriteFile = params.compiledSpriteFile;

        uint16_t bufferWidth = params.bufferWidth;
        uint16_t bufferHeight = params.bufferHeight;

        uint16_t targetPosX = params.targetPosX;
        uint16_t targetPosY = params.targetPosY;

        printf("Loading compiled sprite from file: %s\n", compiledSpriteFile.c_str());
        CompiledSprite sprite(compiledSpriteFile.c_str());
        printf("Loaded compiled sprite of size: %zu bytes\n", sprite.getCompiledCodeSize());

        printf("Creating output buffer of size %dx%d...\n", bufferWidth, bufferHeight);
        printf("Output buffer will be written to: %s\n", outputFile.c_str());
        uint8_t* targetBuffer = new uint8_t[bufferWidth * bufferHeight]; // Example target buffer for a 320x200 screen
        memset(targetBuffer, 0, bufferWidth * bufferHeight); // Clear the buffer

        printf("Executing sprite and rendering to target buffer at position (%d, %d)...\n", targetPosX, targetPosY);
        sprite.execute(targetBuffer + targetPosY * bufferWidth + targetPosX, bufferWidth);
        printf("Sprite executed and rendered to target buffer.\n");

        printf("Writing target buffer to output file: %s\n", outputFile.c_str());
        FILE* outFile = fopen(outputFile.c_str(), "wb");
        if (!outFile) {
            throw std::runtime_error(std::string("Failed to open output file: ") + outputFile);
        }
        size_t bytesWritten = fwrite(targetBuffer, 1, bufferWidth * bufferHeight, outFile);
        if (bytesWritten != bufferWidth * bufferHeight) {
            fclose(outFile);
            throw std::runtime_error(std::string("Failed to write to output file: ") + outputFile);
        }
        fclose(outFile);


        delete[] targetBuffer;

        printf("Test completed successfully.\n");
    }
    catch (const std::exception& e)
    {
        printf("Exception: %s\n", e.what());
        return 1;
    }
    catch (...)
    {
        printf("Unknown exception occurred.\n");
        return 2;
    }
    return 0;
}