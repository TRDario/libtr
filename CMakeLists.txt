cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

project(tr LANGUAGES C CXX VERSION 0.0.1)

add_library(tr STATIC)
target_compile_features(tr PRIVATE cxx_std_20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(tr PRIVATE -Wall -Wpedantic)
    target_compile_options(tr PRIVATE -fno-trapping-math -fno-math-errno -fno-signed-zeros -fassociative-math -ffp-contract=fast)
    target_compile_options(tr PRIVATE -march=x86-64-v2)
endif()
target_sources(tr PRIVATE
    src/audio_buffer.cpp src/audio_context.cpp src/audio_device.cpp src/audio_source.cpp src/audio_stream.cpp
    src/benchmark.cpp src/bitmap_format.cpp src/bitmap.cpp src/display.cpp src/event.cpp src/framebuffer.cpp
    src/gl_buffer.cpp src/gl_context.cpp src/index_buffer.cpp src/iostream.cpp src/keyboard.cpp src/listener.cpp
    src/mouse.cpp src/path.cpp src/renderbuffer.cpp src/sampler.cpp src/sdl.cpp src/shader_buffer.cpp src/shader_pipeline.cpp
    src/shader.cpp src/texture_unit.cpp src/texture.cpp src/ttfont.cpp src/vertex_buffer.cpp src/vertex_format.cpp
    src/vertex.cpp src/window.cpp
)
target_sources(tr PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/tr/dependencies/half.hpp include/tr/dependencies/EnumBitmask.hpp include/tr/dependencies/magic_enum.hpp
        include/tr/angle.hpp include/tr/audio_buffer.hpp include/tr/audio_context.hpp include/tr/audio_device.hpp
        include/tr/audio_source.hpp include/tr/audio_stream.hpp include/tr/benchmark.hpp include/tr/bitmap_format.hpp
        include/tr/bitmap.hpp include/tr/chrono.hpp include/tr/color_cast.hpp include/tr/color.hpp
        include/tr/concepts.hpp include/tr/display.hpp include/tr/draw_geometry.hpp include/tr/event.hpp
        include/tr/framebuffer.hpp include/tr/function.hpp include/tr/geometry.hpp include/tr/gl_buffer.hpp
        include/tr/gl_context.hpp include/tr/handle.hpp include/tr/hashmap.hpp include/tr/index_buffer.hpp
        include/tr/iostream.hpp include/tr/keyboard.hpp include/tr/listener.hpp include/tr/mouse.hpp
        include/tr/norm_cast.hpp include/tr/path.hpp include/tr/ranges.hpp include/tr/renderbuffer.hpp
        include/tr/sampler.hpp include/tr/sdl.hpp include/tr/shader_buffer.hpp include/tr/shader_pipeline.hpp
        include/tr/shader.hpp include/tr/texture_unit.hpp include/tr/texture.hpp include/tr/tr.hpp
        include/tr/ttfont.hpp include/tr/utf8.hpp include/tr/vertex_buffer.hpp include/tr/vertex_format.hpp
        include/tr/vertex.hpp include/tr/window.hpp
)
set_target_properties(tr PROPERTIES DEBUG_POSTFIX d)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build/tr.pc.in ${CMAKE_CURRENT_BINARY_DIR}/tr.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tr.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/tr-config-version.cmake COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tr-config-version.cmake ${CMAKE_CURRENT_SOURCE_DIR}/build/tr-config.cmake DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/tr)
install(TARGETS tr EXPORT trTargets FILE_SET HEADERS)
install(EXPORT trTargets NAMESPACE tr:: DESTINATION ${CMAKE_INSTALL_DATADIR}/tr)