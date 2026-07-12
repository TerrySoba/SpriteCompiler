cmake_minimum_required(VERSION 3.20)

project(SpriteCompiler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

find_package(CLI11 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(sprite_compiler_lib
    src/compiled_sprite.h
    src/compiled_sprite.cpp
    src/animation_json_parser.h
    src/animation_json_parser.cpp
    src/animation_file_writer.h
    src/animation_file_writer.cpp
    src/tga_image.h
    src/tga_image.cpp
    src/exception.cpp
)

target_include_directories(sprite_compiler_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

add_executable(sprite_compiler
    src/main.cpp
)

target_link_libraries(sprite_compiler
    PRIVATE
        CLI11::CLI11
        nlohmann_json::nlohmann_json
        sprite_compiler_lib
)

if(BUILD_TESTING)
    find_package(Catch2 3 REQUIRED)

    add_executable(sprite_compiler_tests
        tests/sprite_compiler_tests.cpp
    )

    add_compile_definitions(sprite_compiler_tests PRIVATE
       TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/tests/test_data"
       PROJECT_BASE_DIR="${PROJECT_SOURCE_DIR}"
    )

    target_link_libraries(sprite_compiler_tests
        PRIVATE
            sprite_compiler_lib
            Catch2::Catch2WithMain
    )

    include(Catch)
    catch_discover_tests(sprite_compiler_tests)
endif()