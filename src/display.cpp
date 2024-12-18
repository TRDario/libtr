#include "../include/tr/display.hpp"
#include <SDL2/SDL.h>

namespace tr {
	// Converts an SDL display mode to a tr one.
	DisplayMode toDisplayMode(const SDL_DisplayMode& mode) noexcept;
} // namespace tr

tr::DisplayMode tr::toDisplayMode(const SDL_DisplayMode& mode) noexcept
{
	return DisplayMode{{mode.w, mode.h}, BitmapFormat::Type(mode.format), mode.refresh_rate};
}

tr::DisplayInfo::DisplayInfo(int id) noexcept
	: _id{id}
{
}

tr::DisplayInfo tr::display(int display) noexcept
{
	assert(display < displayCount());
	return DisplayInfo{display};
}

std::optional<tr::DisplayInfo> tr::display(glm::ivec2 globalCoords) noexcept
{
	for (int i = 0; i < displayCount(); ++i) {
		if (display(i).bounds().contains(globalCoords)) {
			return display(i);
		}
	}
	return std::nullopt;
}

tr::RectI2 tr::DisplayInfo::bounds() const noexcept
{
	SDL_Rect sdlRect;
	SDL_GetDisplayBounds(_id, &sdlRect);
	return {{sdlRect.x, sdlRect.y}, {sdlRect.w, sdlRect.h}};
}

glm::ivec2 tr::DisplayInfo::toLocalCoords(glm::ivec2 globalCoords) const noexcept
{
	return globalCoords - bounds().tl;
}

glm::ivec2 tr::DisplayInfo::toGlobalCoords(glm::ivec2 localCoords) const noexcept
{
	return localCoords + bounds().tl;
}

glm::ivec2 tr::DisplayInfo::centeredPos() const noexcept
{
	return {SDL_WINDOWPOS_CENTERED_DISPLAY(_id), SDL_WINDOWPOS_CENTERED_DISPLAY(_id)};
}

int tr::DisplayInfo::displayModeCount() const noexcept
{
	return SDL_GetNumDisplayModes(_id);
}

tr::DisplayMode tr::DisplayInfo::displayMode(int index) const noexcept
{
	assert(index >= -2 && index < displayModeCount());
	SDL_DisplayMode sdlMode;
	if (index == DESKTOP_MODE) {
		SDL_GetDesktopDisplayMode(_id, &sdlMode);
	}
	else {
		SDL_GetDisplayMode(_id, index == HIGHEST_MODE ? 0 : index, &sdlMode);
	}
	return toDisplayMode(sdlMode);
}

std::optional<tr::DisplayMode> tr::DisplayInfo::closestModeTo(const DisplayMode& mode) const noexcept
{
	SDL_DisplayMode target{std::uint32_t(BitmapFormat::Type(mode.format)), mode.size.x, mode.size.y, mode.refreshRate};
	SDL_DisplayMode sdlMode;
	return SDL_GetClosestDisplayMode(_id, &target, &sdlMode) != nullptr ? std::optional{toDisplayMode(sdlMode)}
																		: std::nullopt;
}

int tr::displayCount() noexcept
{
	return SDL_GetNumVideoDisplays();
}
