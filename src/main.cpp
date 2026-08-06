#include <CLI/CLI.hpp>

#include <nlohmann/json.hpp>

#include "animation_json_parser.h"
#include "compile_animation_frames.h"
#include "animation_file_writer.h"
#include "sprite_format.h"

int main(int argc, char** argv) {
    try
    {
        CLI::App app{
            "This tool takes a TGA image and a JSON animation file "
            "and compiles them into a single binary animation file. "
            "The image and the json file must be exported from Aseprite or LibreSprite. "
            "The output file contains the sprite in form of 8086 machine code that can "
            "be drawn very quickly on a DOS machine."};

        std::string tgaInputFile;
        std::string outputFile;
        int targetWidth;
        std::string formatValue;
        std::string animationFile;

        app.add_option("-i,--input", tgaInputFile, "Input image (TGA image)")->required();
        app.add_option("-a,--animation", animationFile, "Input animation file (JSON)")->required();
        app.add_option("-o,--output", outputFile, "Output file (animation + compiled sprite)")->required();
        app.add_option("-w,--width", targetWidth, "Width of the target buffer")->required();
        app.add_option("-f,--format", formatValue, R"(Sprite format, "automatic", "compiled" or "RLE". Default is "automatic".)")->default_val("automatic");
        
        CLI11_PARSE(app, argc, argv);

        const SpriteFormat format = parseSpriteFormat(formatValue);
        auto animationData = parseAnimationJson(animationFile);
        auto compiledFrames = compileAnimationFrames(tgaInputFile, animationData, targetWidth, format);

        writeAnimationFile(outputFile, animationData, compiledFrames);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred." << std::endl;
        return 2;
    }

	return 0;
}
