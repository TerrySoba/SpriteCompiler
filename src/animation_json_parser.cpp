#include "animation_json_parser.h"


std::string readFileToString(const std::string& filename)
{
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::string content(fileSize, '\0');
    size_t bytesRead = fread(&content[0], 1, fileSize, file);
    if (bytesRead != static_cast<size_t>(fileSize)) {
        fclose(file);
        throw std::runtime_error("Failed to read all data from file: " + filename);
    }

    fclose(file);
    return content;
}

AnimationData parseAnimationJson(const std::string& jsonFilename)
{
    std::string jsonString = readFileToString(jsonFilename);
    auto jsonData = nlohmann::ordered_json::parse(jsonString);

    AnimationData animationData;

    // Parse frames
    for (const auto& frame : jsonData["frames"].items()) {
        FrameData frameData;
        frameData.x = frame.value()["frame"]["x"];
        frameData.y = frame.value()["frame"]["y"];
        frameData.w = frame.value()["frame"]["w"];
        frameData.h = frame.value()["frame"]["h"];

        // printf("Parsed frame: x=%d, y=%d, w=%d, h=%d\n", frameData.x, frameData.y, frameData.w, frameData.h); // Debug print

        animationData.frames.push_back(frameData);
    }

    // Parse tags
    for (const auto& tag : jsonData["meta"]["frameTags"]) {
        AnimationTag animationTag;
        animationTag.name = tag["name"];
        animationTag.from = tag["from"];
        animationTag.to = tag["to"];
        animationData.tags.push_back(animationTag);
    }

    return animationData;
}