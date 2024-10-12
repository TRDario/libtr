module;
#include <cassert>
#include <SDL2/SDL.h>

export module tr:display;

import std;
import glm;
import :geometry;
import :bitmap_format;

export namespace tr {
	// Struct representing a display mode.
	struct DisplayMode {
		glm::ivec2  size;
		BitmapFormat format;
		int 		refreshRate;
	};

	// Class for querying display info.
	class DisplayInfo {

	public:
		constexpr DisplayInfo() noexcept : _id { 0 } {}
		DisplayInfo(int id) noexcept;


		// Tries to return a display info from global coordinates.
		static std::optional<DisplayInfo> fromCoords(glm::ivec2 globalCoords) noexcept;

		// Gets the bounds of the display.
		RectI2 bounds() const noexcept;
		// Returns a sentinel value used by the window's positioning function to center itself on the display.
		glm::ivec2 centeredPos() const noexcept;

		// Converts global coordinates to local coordinates on the display.
		glm::ivec2 toLocalCoords(glm::ivec2 globalCoords) const noexcept;
		// Converts local coordinates on the display to global coordinates.
		glm::ivec2 toGlobalCoords(glm::ivec2 localCoords) const noexcept;

		// Gets the display's display modes.
		std::vector<DisplayMode> modes() const;
		// Gets the display's highest-ordered display mode.
		DisplayMode highestMode() const noexcept;
		// Gets the display's desktop display mode.
		DisplayMode desktopMode() const noexcept;
		// Gets the closest compatible display mode to the desired one.
		std::optional<DisplayMode> closestModeTo(const DisplayMode& mode) const noexcept;
	private:
		int _id;
	};

	// A reference to the default display.
	inline constexpr DisplayInfo defaultDisplay {};

	// Gets the number of video displays.
	int displayCount() noexcept;
}

// IMPLEMENTATION

namespace tr {
    // Converts an SDL display mode to a tr one.
    DisplayMode toDisplayMode(const SDL_DisplayMode& mode) noexcept;
}

tr::DisplayMode tr::toDisplayMode(const SDL_DisplayMode& mode) noexcept
{
    return DisplayMode { { mode.w, mode.h }, BitmapFormat::Type(mode.format), mode.refresh_rate };
}

tr::DisplayInfo::DisplayInfo(int id) noexcept
    : _id { id }
{
    assert(id < displayCount());
}

std::optional<tr::DisplayInfo> tr::DisplayInfo::fromCoords(glm::ivec2 globalCoords) noexcept
{
	for (int i = 0; i < displayCount(); ++i) {
		if (within(globalCoords, DisplayInfo { i }.bounds())) {
			return DisplayInfo { i };
		}
	}
	return std::nullopt;
}

tr::RectI2 tr::DisplayInfo::bounds() const noexcept
{
    SDL_Rect sdlRect;
    SDL_GetDisplayBounds(_id, &sdlRect);
    return { { sdlRect.x, sdlRect.y }, { sdlRect.w, sdlRect.h } };
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
    return { SDL_WINDOWPOS_CENTERED_DISPLAY(_id), SDL_WINDOWPOS_CENTERED_DISPLAY(_id) };
}

std::vector<tr::DisplayMode> tr::DisplayInfo::modes() const
{
    auto nmodes { SDL_GetNumDisplayModes(_id) };
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
    SDL_DisplayMode target { std::uint32_t(BitmapFormat::Type(mode.format)), mode.size.x, mode.size.y, mode.refreshRate };
    SDL_DisplayMode sdlMode;
    return SDL_GetClosestDisplayMode(_id, &target, &sdlMode) != nullptr ? std::optional { toDisplayMode(sdlMode) } : std::nullopt;
}

int tr::displayCount() noexcept
{
    return SDL_GetNumVideoDisplays();
}