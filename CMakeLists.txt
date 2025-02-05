cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

project(tr LANGUAGES CXX VERSION 0.1.0)

option(TR_ENABLE_INSTALL "whether to enable the install rule" ON)
option(TR_DEAR_IMGUI_INTEGRATION "whether to include Dear ImGui integration" OFF)

include(FetchContent)

find_package(OpenAL REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(magic_enum REQUIRED)
find_package(OpenAL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SndFile REQUIRED)

add_library(tr STATIC)
target_compile_features(tr PRIVATE cxx_std_20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(tr PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(tr PRIVATE -fno-trapping-math -fno-math-errno -fno-signed-zeros -fassociative-math -ffp-contract=fast)
    target_compile_options(tr PRIVATE -march=native)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(tr PRIVATE /W4 /WX)
endif()
target_link_libraries(tr PUBLIC SDL2_image SDL2_ttf SDL2 OpenGL openal sndfile)
set_target_properties(tr PROPERTIES DEBUG_POSTFIX "d")

target_sources(tr PRIVATE
    src/audio_buffer.cpp src/audio_source.cpp src/audio_system.cpp
    src/benchmark.cpp src/bitmap_format.cpp src/bitmap_iterators.cpp src/bitmap.cpp src/display.cpp src/event.cpp
    src/framebuffer.cpp src/graphics_buffer.cpp src/glad.cpp src/graphics_context.cpp src/index_buffer.cpp src/iostream.cpp src/keyboard.cpp
    src/listener.cpp src/mouse.cpp src/path.cpp src/rng.cpp src/sdl.cpp src/shader_buffer.cpp
    src/shader_pipeline.cpp src/shader.cpp src/stopwatch.cpp src/texture_unit.cpp src/texture.cpp src/timer.cpp src/ttfont.cpp
    src/vertex_buffer.cpp src/vertex_format.cpp src/vertex.cpp src/window.cpp
)
target_sources(tr PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/tr/dependencies/EnumBitmask.hpp include/tr/dependencies/half.hpp include/tr/dependencies/glad.h include/tr/dependencies/khrplatform.h
        include/tr/angle_impl.hpp include/tr/angle.hpp include/tr/audio_buffer.hpp include/tr/audio_source.hpp
        include/tr/audio_system.hpp include/tr/benchmark.hpp include/tr/bitmap_format.hpp include/tr/bitmap_iterators.hpp
        include/tr/bitmap.hpp include/tr/chrono.hpp include/tr/color_cast.hpp include/tr/chrono.hpp include/tr/color_cast_impl.hpp
        include/tr/color.hpp include/tr/common.hpp include/tr/concepts.hpp include/tr/display.hpp include/tr/draw_geometry_impl.hpp
        include/tr/draw_geometry.hpp include/tr/event.hpp include/tr/framebuffer.hpp include/tr/geometry_impl.hpp
        include/tr/geometry.hpp include/tr/graphics_buffer.hpp include/tr/graphics_context.hpp include/tr/handle.hpp include/tr/hashmap.hpp
        include/tr/index_buffer.hpp include/tr/iostream.hpp include/tr/keyboard.hpp include/tr/listener.hpp include/tr/mouse.hpp
        include/tr/norm_cast.hpp include/tr/overloaded_lambda.hpp include/tr/path.hpp include/tr/ranges.hpp include/tr/rng.hpp
        include/tr/rng_impl.hpp include/tr/sdl.hpp include/tr/shader_buffer.hpp
        include/tr/shader_pipeline.hpp include/tr/shader.hpp include/tr/stopwatch.hpp include/tr/texture_unit.hpp
        include/tr/texture.hpp include/tr/timer.hpp include/tr/tr.hpp include/tr/ttfont.hpp include/tr/utf8.hpp
        include/tr/vertex_buffer.hpp include/tr/vertex_format.hpp include/tr/vertex.hpp include/tr/window.hpp
)
target_precompile_headers(tr PRIVATE include/tr/color.hpp include/tr/geometry.hpp include/tr/ranges.hpp)

if(TR_DEAR_IMGUI_INTEGRATION)
    target_sources(tr PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/tr/imgui.hpp
    )
    target_sources(tr PRIVATE
        src/imgui/imgui_impl_opengl3.cpp src/imgui/imgui_impl_sdl2.cpp src/imgui.cpp
    )
endif()

if(TR_ENABLE_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS tr
        EXPORT trTargets
        FILE_SET HEADERS
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(
        EXPORT trTargets
        NAMESPACE tr::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/tr"
    )

    configure_package_config_file(
        cmake/trConfig.cmake.in
        "${PROJECT_BINARY_DIR}/trConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/tr"
    )
    install(
        FILES "${PROJECT_BINARY_DIR}/trConfig.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/tr"
    )

    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/trConfigVersion.cmake"
        COMPATIBILITY SameMinorVersion)
    install(
        FILES "${PROJECT_BINARY_DIR}/trConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/tr"
    )
endif()