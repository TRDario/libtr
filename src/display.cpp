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
	assert(id < displayCount());
}

std::optional<tr::DisplayInfo> tr::DisplayInfo::fromCoords(glm::ivec2 globalCoords) noexcept
{
	for (int i = 0; i < displayCount(); ++i) {
		if (DisplayInfo{i}.bounds().contains(globalCoords)) {
			return DisplayInfo{i};
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

std::vector<tr::DisplayMode> tr::DisplayInfo::modes() const
{
	auto nmodes{SDL_GetNumDisplayModes(_id)};
	std::vector<DisplayMode> modes;
	modes.reserve(nmodes);
	for (int i = 0; i < nmodes; ++i) {
		SDL_DisplayMode sdlMode;
		SDL_GetDisplayMode(_id, i, &sdlMode);
		modes.push_back(toDisplayMode(sdlMode));
	}
	return modes;
}

tr::DisplayMode tr::DisplayInfo::highestMode() const noexcept
{
	SDL_DisplayMode sdlMode;
	SDL_GetDisplayMode(_id, 0, &sdlMode);
	return toDisplayMode(sdlMode);
}

tr::DisplayMode tr::DisplayInfo::desktopMode() const noexcept
{
	SDL_DisplayMode sdlMode;
	SDL_GetDesktopDisplayMode(_id, &sdlMode);
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
