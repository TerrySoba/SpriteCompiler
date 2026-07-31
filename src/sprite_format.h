#pragma once

#include <stdexcept>
#include <string>
#include <algorithm>

enum class SpriteFormat
{
    Compiled,
    Rle,
    Automatic // use  format that produces smaller output
};

inline SpriteFormat parseSpriteFormat(const std::string& value)
{
    // convert value to uppercase for case-insensitive comparison
    auto upperValue = value;
    std::transform(upperValue.begin(), upperValue.end(), upperValue.begin(), ::toupper);
    if (upperValue == "COMPILED")
    {
        return SpriteFormat::Compiled;
    }

    if (upperValue == "RLE")
    {
        return SpriteFormat::Rle;
    }

    if (upperValue == "AUTOMATIC")
    {
        return SpriteFormat::Automatic;
    }
    throw std::invalid_argument("Unsupported sprite format: " + value);
}

inline const char* spriteFormatToString(SpriteFormat value)
{
    switch (value)
    {
        case SpriteFormat::Compiled:
            return "compiled";
        case SpriteFormat::Rle:
            return "rle";
        case SpriteFormat::Automatic:
            return "automatic";
    }

    return "unknown";
}
