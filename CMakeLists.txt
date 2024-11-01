cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

project(tr LANGUAGES C CXX)

if (NOT TR_ENUM_BITMASK_INCLUDE_DIR)
    message(FATAL_ERROR "EnumBitmask.hpp directory not specified!")
endif()
if (NOT TR_MAGIC_ENUM_INCLUDE_DIR)
    message(FATAL_ERROR "magic_enum directory not specified!")
endif()

find_package(Boost REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenAL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SndFile REQUIRED)

add_library(tr STATIC)

target_compile_features(tr PUBLIC cxx_std_20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(tr PRIVATE -Wall -Wpedantic)
    target_compile_options(tr PRIVATE -fno-trapping-math -fno-math-errno -fno-signed-zeros -fassociative-math -ffp-contract=fast)
    target_compile_options(tr PRIVATE -march=x86-64-v2)
endif()

target_sources(tr PRIVATE
    src/audio_buffer.cpp src/audio_context.cpp src/audio_device.cpp src/audio_source.cpp src/audio_stream.cpp
    src/benchmark.cpp src/bitmap_format.cpp src/bitmap.cpp src/display.cpp src/event.cpp src/framebuffer.cpp
    src/gl_buffer.cpp src/gl_context.cpp src/index_buffer.cpp src/iostream.cpp src/keyboard.cpp src/listener.cpp
    src/mouse.cpp src/path.cpp src/sampler.cpp src/sdl.cpp src/shader_buffer.cpp src/shader_pipeline.cpp src/shader.cpp
    src/texture_unit.cpp src/texture.cpp src/ttfont.cpp src/vertex_format.cpp src/vertex.cpp src/window.cpp
)
target_sources(tr PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/angle.hpp include/audio_buffer.hpp include/audio_context.hpp include/audio_device.hpp
        include/audio_source.hpp include/audio_stream.hpp include/benchmark.hpp include/bitmap_format.hpp
        include/bitmap.hpp include/chrono.hpp include/color_cast.hpp include/color.hpp include/concepts.hpp
        include/display.hpp include/draw_geometry.hpp include/event.hpp include/framebuffer.hpp include/function.hpp
        include/geometry.hpp include/gl_buffer.hpp include/gl_context.hpp include/handle.hpp include/hashmap.hpp
        include/index_buffer.hpp include/iostream.hpp include/keyboard.hpp include/listener.hpp include/mouse.hpp
        include/norm_cast.hpp include/path.hpp include/ranges.hpp include/renderbuffer.hpp include/sampler.hpp
        include/sdl.hpp include/shader_buffer.hpp include/shader_pipeline.hpp include/shader.hpp
        include/texture_unit.hpp include/texture.hpp include/tr.hpp include/ttfont.hpp include/utf8.hpp
        include/vertex_buffer.hpp include/vertex_format.hpp include/vertex.hpp include/window.hpp
)
target_include_directories(tr PRIVATE ${TR_ENUM_BITMASK_INCLUDE_DIR} ${TR_MAGIC_ENUM_INCLUDE_DIR})
set_target_properties(tr PROPERTIES DEBUG_POSTFIX d)