#include <CLI/CLI.hpp>

#include <nlohmann/json.hpp>

#include "animation_json_parser.h"
#include "compiled_sprite.h"
#include "animation_file_writer.h"

int main(int argc, char** argv) {
	CLI::App app{"SpriteCompiler"};

	// bool show_version = false;
	// app.add_flag("-v,--version", show_version, "Print version information");
    std::string tgaInputFile;
    std::string outputFile;
    int targetWidth;
    std::string animationFile;

    app.add_option("-i,--input", tgaInputFile, "Input file (TGA image)")->required();
    app.add_option("-o,--output", outputFile, "Output file (compiled sprite)")->required();
    app.add_option("-w,--width", targetWidth, "Width of the target buffer")->required();
    app.add_option("-a,--animation", animationFile, "Animation file")->required();

	CLI11_PARSE(app, argc, argv);

    auto animationData = parseAnimationJson(animationFile);
    writeAnimationFile(outputFile, animationData);

	return 0;
}
