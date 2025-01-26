#include "../include/tr/display.hpp"
#include <SDL2/SDL.h>

glm::ivec2 tr::displaySize() noexcept
{
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);
	return {mode.w, mode.h};
}

int tr::refreshRate() noexcept
{
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);
	return mode.refresh_rate;
}