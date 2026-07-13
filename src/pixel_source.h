#ifndef PIXEL_SOURCE_H
#define PIXEL_SOURCE_H

#include <stdint.h>

class PixelSource
{
public:
    virtual uint16_t width() const = 0;
    virtual uint16_t height() const = 0;
    virtual char pixel(uint16_t x, uint16_t y) const = 0;
    virtual char transparentColor() const = 0;
};


#endif
