#include <CLI/CLI.hpp>

#include <nlohmann/json.hpp>

#include "animation_json_parser.h"
#include "compile_animation_frames.h"
#include "animation_file_writer.h"

int main(int argc, char** argv) {
    try
    {
        CLI::App app{"SpriteCompiler"};

        std::string tgaInputFile;
        std::string outputFile;
        int targetWidth;
        std::string animationFile;

        app.add_option("-i,--input", tgaInputFile, "Input image (TGA image)")->required();
        app.add_option("-a,--animation", animationFile, "Input animation file (JSON)")->required();
        app.add_option("-o,--output", outputFile, "Output file (animation + compiled sprite)")->required();
        app.add_option("-w,--width", targetWidth, "Width of the target buffer")->required();
        
        CLI11_PARSE(app, argc, argv);

        auto animationData = parseAnimationJson(animationFile);
        auto compiledFrames = compileAnimationFrames(tgaInputFile, animationData, targetWidth);

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
