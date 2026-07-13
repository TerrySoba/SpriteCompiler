#ifndef TGA_READER_H_INCLUDED
#define TGA_READER_H_INCLUDED

#include "image_base.h"

#include <stdint.h>

class TgaImage : public ImageBase
{
public:
    TgaImage(const char* filename);

    void loadImage(const char* filename);

    virtual ~TgaImage();

    virtual uint16_t width() const;
	virtual uint16_t height() const;
	virtual char* data() const;

private:
    char* m_data;
    uint32_t m_dataSize;
    uint16_t m_width;
    uint16_t m_height;
};


#endif