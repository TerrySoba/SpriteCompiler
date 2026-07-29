#include <catch2/catch_all.hpp>

#include "sprite_format.h"

TEST_CASE("Sprite format parser accepts supported names", "[sprite_format]")
{
    REQUIRE(parseSpriteFormat("compiled") == SpriteFormat::Compiled);
    REQUIRE(parseSpriteFormat("RLE") == SpriteFormat::Rle);
    REQUIRE(parseSpriteFormat("rle") == SpriteFormat::Rle);
}

TEST_CASE("Sprite format parser rejects unsupported values", "[sprite_format]")
{
    REQUIRE_THROWS_AS(parseSpriteFormat("invalid"), std::invalid_argument);
}
