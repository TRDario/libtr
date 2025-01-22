#include "../include/tr/sdl.hpp"
#include <SDL3/SDL.h>
#include <format>

tr::SDLError::SDLError(std::string_view message)
	: runtime_error{std::format("{} ({})", message, SDL_GetError())}
{
}
