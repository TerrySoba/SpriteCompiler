#ifndef _COMPILED_SPRITE_H_INCLUDED
#define _COMPILED_SPRITE_H_INCLUDED

#include "pixel_source.h"

#include <stdint.h>
#include <vector>


/**
 * A compiled sprite loads the given tga file and compiles it to
 * 8086 machine code. This makes drawing it faster, especially if
 * it contains transparency, bit it has an overhead of 2 to 4 bytes
 * per pixel in ram, depending on image content.
 * 
 * The target image width has to be known at construction time of the
 * sprite.
 * 
 * To get the assembler bytecodes I used the disassembler of
 * open watcom "wdis".
 */
class CompiledSprite
{
public:
    CompiledSprite(const char* filename, uint16_t targetWidth);
    CompiledSprite(const PixelSource& image, uint16_t targetWidth);
    virtual ~CompiledSprite();

    virtual uint16_t width() const;
    virtual uint16_t height() const;

    const std::vector<char>& getCompiledFunction() const
    {
        return m_compiledFunction;
    }

    const std::vector<char>& getOriginalData() const
    {
        return m_originalData;
    }

private:
    std::vector<char> compileSprite(const PixelSource& image, uint16_t targetWidth);

private:
    uint16_t m_width, m_height;
    std::vector<char> m_compiledFunction;
    std::vector<char> m_originalData;
};

typedef void (*DrawCompiledSpritePtr)(char* img, uint16_t targetWidth);

// uint32_t compileData(char* dst, uint32_t dstSize, const PixelSource& image, uint16_t targetWidth);

#endif