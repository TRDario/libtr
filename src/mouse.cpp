#include "../include/mouse.hpp"
#include <SDL2/SDL.h>

namespace tr {
    // Checks if a cursor if not null and throws CursorBadAlloc otherwise.
    SDL_Cursor* checkNotNull(SDL_Cursor* ptr);
}


SDL_Cursor* tr::checkNotNull(SDL_Cursor* ptr)
{
    if (ptr == nullptr) {
        throw CursorBadAlloc {};
    }
    return ptr;
}

const char* tr::CursorBadAlloc::what() const noexcept {
    return "failed cursor allocation";
}

glm::ivec2 tr::mouse::pos() noexcept
{
    glm::ivec2 pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

glm::ivec2 tr::mouse::delta() noexcept
{
    glm::ivec2 delta;
    SDL_GetRelativeMouseState(&delta.x, &delta.y);
    return delta;
}

tr::MouseButtonMask tr::mouse::buttons() noexcept
{
    return MouseButtonMask(SDL_GetMouseState(nullptr, nullptr));
}

bool tr::mouse::held(MouseButton button) noexcept
{
    return held(button, buttons());
}

bool tr::mouse::held(MouseButton button, MouseButtonMask mask) noexcept
{
    return std::uint32_t(mask) & SDL_BUTTON(std::uint32_t(button));
}

bool tr::mouse::inRelativeMode() noexcept
{
    return SDL_GetRelativeMouseMode();
}

bool tr::mouse::setRelativeMode(bool relative) noexcept
{
    return !SDL_SetRelativeMouseMode(SDL_bool(relative));
}

bool tr::mouse::setCaptured(bool captured) noexcept
{
    return !SDL_CaptureMouse(SDL_bool(captured));
}

tr::Cursor::Cursor()
    : _impl { checkNotNull(SDL_GetDefaultCursor()) }
{}

tr::Cursor::Cursor(SysCursor icon)
    : _impl { checkNotNull(SDL_CreateSystemCursor(SDL_SystemCursor(icon))) }
{}

tr::Cursor::Cursor(const Bitmap& bitmap, glm::ivec2 focus)
    : _impl { checkNotNull(SDL_CreateColorCursor(bitmap._impl.get(), focus.x, focus.y)) }
{}

tr::Cursor::Cursor(std::span<const std::byte> color, std::span<const std::byte> mask, glm::ivec2 size, glm::ivec2 focus)
{
    assert(color.size() == mask.size() && color.size() == size.x * size.y / 64);
    _impl.reset(checkNotNull(SDL_CreateCursor((const std::uint8_t*)(color.data()), (const std::uint8_t*)(mask.data()), size.x, size.y, focus.x, focus.y)));
}

void tr::Cursor::Deleter::operator()(SDL_Cursor* ptr) const noexcept {
    SDL_FreeCursor(ptr);
}

void tr::mouse::setCursor(const Cursor& cursor) noexcept
{
    SDL_SetCursor(cursor._impl.get());
}

bool tr::mouse::cursorShown() noexcept
{
    return SDL_ShowCursor(SDL_QUERY);
}

void tr::mouse::showCursor(bool show) noexcept
{
    SDL_ShowCursor(show);
}