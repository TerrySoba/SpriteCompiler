#include "compile_animation_frames.h"

#include "tga_image.h"
#include "compiled_sprite.h"
#include "rle_compression.h"

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
    uint16_t targetWidth,
    SpriteFormat format)
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

        std::vector<char> rleDataBuffer = {'R'}; // 'R' is for RLE
        std::vector<char> compiledDataBuffer = {'C'}; // 'C' is for Compiled

        {
            FramePixelSource frameSource(img.data(), img.width(), frame);
            std::vector<uint8_t> inputData;
            for (uint16_t y = 0; y < frame.h; ++y)
            {
                for (uint16_t x = 0; x < frame.w; ++x)
                {
                    inputData.push_back(static_cast<uint8_t>(frameSource.pixel(x, y)));
                }
            }
            auto compressedChunks = compressRLE(inputData, frame.w, frame.h, frameSource.transparentColor());
            auto encodedData = encodeRLE(compressedChunks);
            rleDataBuffer.insert(rleDataBuffer.end(), encodedData.begin(), encodedData.end());
        }

        {
            FramePixelSource frameSource(img.data(), img.width(), frame);
            CompiledSprite compiledSprite(frameSource, targetWidth);
            compiledDataBuffer.insert(compiledDataBuffer.end(), compiledSprite.getCompiledFunction().begin(), compiledSprite.getCompiledFunction().end());
        }
        
        if (format == SpriteFormat::Automatic)
        {
            if (rleDataBuffer.size() < compiledDataBuffer.size())
            {
                compiledFrames.push_back(rleDataBuffer);
            }
            else
            {
                compiledFrames.push_back(compiledDataBuffer);
            }
        }
        else if (format == SpriteFormat::Rle)
        {
            compiledFrames.push_back(rleDataBuffer);
        }
        else if (format == SpriteFormat::Compiled)
        {
            compiledFrames.push_back(compiledDataBuffer);
        }
    }

    return compiledFrames;
}
