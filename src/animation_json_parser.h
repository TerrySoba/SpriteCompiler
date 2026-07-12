#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include <cstdint>

struct FrameData
{
    uint16_t x, y, w, h;
};

struct AnimationTag
{
    std::string name;
    uint16_t from, to;
};

struct AnimationData
{
    std::vector<FrameData> frames;
    std::vector<AnimationTag> tags;
};

AnimationData parseAnimationJson(const std::string& jsonFilename);