# libtr
libtr is a personal C++ base library for gamedev projects.

libtr includes C++ wrappers for OpenAL, OpenGL 4.6, and SDL2 objects, as well as a number of miscellaneous helper functions, classes, and templates.

## Important notice ##

If you plan to use this library for any reason, it is important to stress that it is currently in unstable development and the API may change on a whim until I deem it stable. There may also be bugs I haven't fixed yet.

## Documentation ##

Documentation can be built with Doxygen, or can be viewed [here](https://trdario.github.io/libtr/).

Currently work-in-progress.

## Examples ##

TO-DO.

## Dependencies ##
libtr depends on the following external libraries:
- [Boost](https://www.boost.org/)
- [EnumBitmask](https://github.com/Reputeless/EnumBitmask)
- [GLM](https://github.com/g-truc/glm)
- [GLEW](https://glew.sourceforge.net/)
- [magic_enum](https://github.com/Neargye/magic_enum)
- [OpenALSoft](https://github.com/kcat/openal-soft)
- [SDL2](https://github.com/libsdl-org/SDL) (including [SDL2_image](https://github.com/libsdl-org/SDL_image) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf))
- [libsndfile](https://github.com/libsndfile/libsndfile)

## Building ##
libtr requires a C++20-compatible compiler. Files for building with CMake are provided.