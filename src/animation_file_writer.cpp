#include "animation_file_writer.h"

#include <cstdio>
#include <stdexcept>

void writeAnimationFile(const std::string& filename, const AnimationData& animationData)
{
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    fwrite("ANIM", sizeof(char), 4, file); // Write a simple header to identify the file type

    // Write the number of frames
    uint16_t numFrames = static_cast<uint16_t>(animationData.frames.size());
    fwrite(&numFrames, sizeof(numFrames), 1, file);

    // Write each frame's data
    for (const auto& frame : animationData.frames) {
        fwrite(&frame.x, sizeof(frame.x), 1, file);
        fwrite(&frame.y, sizeof(frame.y), 1, file);
        fwrite(&frame.w, sizeof(frame.w), 1, file);
        fwrite(&frame.h, sizeof(frame.h), 1, file);
    }

    // Write the number of tags
    uint16_t numTags = static_cast<uint16_t>(animationData.tags.size());
    fwrite(&numTags, sizeof(numTags), 1, file);

    // printf("Writing %d tags to file.\n", numTags); // Debug print
    // Write each tag's data
    for (const auto& tag : animationData.tags) {
        // printf("Writing tag: name='%s', from=%d, to=%d\n", tag.name.c_str(), tag.from, tag.to); // Debug print
        fwrite(&tag.from, sizeof(tag.from), 1, file);
        fwrite(&tag.to, sizeof(tag.to), 1, file);
        uint16_t nameLength = static_cast<uint16_t>(tag.name.size());
        fwrite(&nameLength, sizeof(nameLength), 1, file);
        fwrite(tag.name.c_str(), sizeof(char), nameLength, file);
    }

    fclose(file);
}


