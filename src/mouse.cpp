#include "../include/tr/bitmap.hpp"
#include "../include/tr/mouse.hpp"
#include <SDL2/SDL.h>

namespace tr {
	// Checks if a cursor if not null and throws CursorBadAlloc otherwise.
	SDL_Cursor* checkNotNull(SDL_Cursor* ptr);
} // namespace tr

SDL_Cursor* tr::checkNotNull(SDL_Cursor* ptr)
{
	if (ptr == nullptr) {
		throw CursorBadAlloc{};
	}
	return ptr;
}

const char* tr::CursorBadAlloc::what() const noexcept
{
	return "failed cursor allocation";
}

glm::ivec2 tr::Mouse::position() const noexcept
{
	glm::ivec2 pos;
	SDL_GetMouseState(&pos.x, &pos.y);
	return pos;
}

glm::ivec2 tr::Mouse::delta() const noexcept
{
	glm::ivec2 delta;
	SDL_GetRelativeMouseState(&delta.x, &delta.y);
	return delta;
}

tr::MouseButtonMask tr::Mouse::buttons() const noexcept
{
	return MouseButtonMask(SDL_GetMouseState(nullptr, nullptr));
}

bool tr::Mouse::held(MouseButton button) const noexcept
{
	return held(button, buttons());
}

bool tr::Mouse::held(MouseButton button, MouseButtonMask mask) const noexcept
{
	return static_cast<std::uint32_t>(mask) & SDL_BUTTON(static_cast<std::uint32_t>(button));
}

bool tr::Mouse::inRelativeMode() const noexcept
{
	return SDL_GetRelativeMouseMode();
}

bool tr::Mouse::setRelativeMode(bool relative) noexcept
{
	return !SDL_SetRelativeMouseMode(static_cast<SDL_bool>(relative));
}

bool tr::Mouse::setCaptured(bool captured) noexcept
{
	return !SDL_CaptureMouse(static_cast<SDL_bool>(captured));
}

tr::Cursor::Cursor()
	: _impl{checkNotNull(SDL_GetDefaultCursor())}
{
}

tr::Cursor::Cursor(SysCursor icon)
	: _impl{checkNotNull(SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(icon)))}
{
}

tr::Cursor::Cursor(const Bitmap& bitmap, glm::ivec2 focus)
	: _impl{checkNotNull(SDL_CreateColorCursor(bitmap._impl.get(), focus.x, focus.y))}
{
}

tr::Cursor::Cursor(const BitmapView& view, glm::ivec2 focus)
	: _impl{checkNotNull(SDL_CreateColorCursor(view._impl.get(), focus.x, focus.y))}
{
}

tr::Cursor::Cursor(std::span<const std::byte> color, std::span<const std::byte> mask, glm::ivec2 size, glm::ivec2 focus)
{
	assert(color.size() == mask.size() && color.size() == static_cast<std::size_t>(size.x) * size.y / 64);
	_impl.reset(checkNotNull(SDL_CreateCursor(reinterpret_cast<const std::uint8_t*>(color.data()),
											  reinterpret_cast<const std::uint8_t*>(mask.data()), size.x, size.y,
											  focus.x, focus.y)));
}

void tr::Cursor::Deleter::operator()(SDL_Cursor* ptr) const noexcept
{
	SDL_FreeCursor(ptr);
}

void tr::Mouse::setCursor(const Cursor& cursor) noexcept
{
	SDL_SetCursor(cursor._impl.get());
}

bool tr::Mouse::cursorShown() const noexcept
{
	return SDL_ShowCursor(SDL_QUERY);
}

void tr::Mouse::showCursor(bool show) noexcept
{
	SDL_ShowCursor(show);
}
