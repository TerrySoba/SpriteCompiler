#pragma once

#include "animation_json_parser.h"
#include "sprite_format.h"

#include <string>
#include <vector>

std::vector<std::vector<char>> compileAnimationFrames(
    const std::string& tgaInputFile,
    const AnimationData& animationData,
    uint16_t targetWidth,
    SpriteFormat format);
