# libtr
libtr is a personal C++ base library for gamedev projects.

libtr includes C++ wrappers for OpenAL, OpenGL 4.6, and SDL2 objects, as well as a number of miscellaneous helper functions, classes, and templates.

## Documentation ##

Documentation can be built with Doxygen, or can be viewed [here](https://trdario.github.io/libtr/).

## Examples ##

(Work in progress...)

## Dependencies ##
libtr depends on the following external libraries:
- [Boost](https://www.boost.org/)
- [EnumBitmask](https://github.com/Reputeless/EnumBitmask) (vendored)
- [GLM](https://github.com/g-truc/glm)
- [GLEW](https://glew.sourceforge.net/)
- [half](https://sourceforge.net/projects/half/files/half/) (vendored)
- [magic_enum](https://github.com/Neargye/magic_enum)
- [OpenALSoft](https://github.com/kcat/openal-soft)
- [SDL2](https://github.com/libsdl-org/SDL) (including [SDL2_image](https://github.com/libsdl-org/SDL_image) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf))
- [libsndfile](https://github.com/libsndfile/libsndfile)

## Building ##
The following is required to build libtre:
- A C++20 compiler.
- CMake 3.23.0 or higher.

libtr can be easily integrated into a project using CMake FetchContent:
```
include(FetchContent)
FetchContent_Declare(tr GIT_REPOSITORY https://github.com/TRDario/libtr.git GIT_TAG main FIND_PACKAGE_ARGS NAMES tr)
FetchContent_MakeAvailable(tr)
```