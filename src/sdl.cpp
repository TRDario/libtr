#include "../include/tr/sdl.hpp"
#include <SDL2/SDL.h>

tr::SDLError::SDLError(std::string_view message)
	: runtime_error{std::format("{} ({})", message, SDL_GetError())}
{
}
