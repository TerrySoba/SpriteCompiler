#include "compile_animation_frames.h"

#include "tga_image.h"
#include "compiled_sprite.h"

class FramePixelSource : public PixelSource
{
public:
    FramePixelSource(const char* data, uint16_t imageWidth, FrameData frame) :
        m_data(data), m_frame(frame), m_imageWidth(imageWidth)
    {
    }

    virtual uint16_t width() const
    {
        return m_frame.w;
    }

    virtual uint16_t height() const
    {
        return m_frame.h;
    }

    virtual char pixel(uint16_t x, uint16_t y) const
    {
        return m_data[m_imageWidth * (m_frame.y + y) + (m_frame.x + x)];
    }

    virtual char transparentColor() const
    {
        return 0;
    }

private:
    const char* m_data;
    FrameData m_frame;
    uint16_t m_imageWidth;
};


std::vector<std::vector<char>> compileAnimationFrames(
    const std::string& tgaInputFile,
    const AnimationData& animationData,
    uint16_t targetWidth)
{
    // load the TGA image
    TgaImage img(tgaInputFile.c_str());

    for (const auto& frame : animationData.frames)
    {
        if (frame.x + frame.w > img.width() || frame.y + frame.h > img.height())
        {
            throw std::runtime_error("Frame dimensions exceed image dimensions.");
        }
    }

    std::vector<std::vector<char>> compiledFrames;
    for (const auto& frame : animationData.frames)
    {
        FramePixelSource frameSource(img.data(), img.width(), frame);
        CompiledSprite compiledSprite(frameSource, targetWidth);
        compiledFrames.push_back(compiledSprite.getCompiledFunction()); 
    }

    return compiledFrames;
}
