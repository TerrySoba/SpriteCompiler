cmake_minimum_required(VERSION 3.20)

project(SpriteCompiler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)


add_library(sprite_compiler
    src/compiled_sprite.h
    src/compiled_sprite.cpp
    src/tga_image.h
    src/tga_image.cpp
    src/exception.cpp
)

target_include_directories(sprite_compiler
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
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
            sprite_compiler
            Catch2::Catch2WithMain
    )

    include(Catch)
    catch_discover_tests(sprite_compiler_tests)
endif()