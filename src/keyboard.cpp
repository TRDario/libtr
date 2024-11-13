#include "../include/tr/keyboard.hpp"

#include <SDL2/SDL.h>
#include <cassert>

tr::Scancode::Scancode(Enum base) noexcept
	: _enum{base}
{
}

tr::Scancode::Scancode(const char* name) noexcept
	: _enum{Enum(SDL_GetScancodeFromName(name))}
{
}

tr::Scancode::operator Enum() const noexcept
{
	return _enum;
}

tr::Scancode::operator Keycode() const noexcept
{
	return Keycode::Enum(SDL_GetKeyFromScancode(SDL_Scancode(_enum)));
}

const char* tr::Scancode::name() const noexcept
{
	return SDL_GetScancodeName(SDL_Scancode(_enum));
}

tr::Keycode::Keycode(Enum base) noexcept
	: _enum{base}
{
}

tr::Keycode::Keycode(const char* name) noexcept
	: _enum{Enum(SDL_GetKeyFromName(name))}
{
}

tr::Keycode::operator Enum() const noexcept
{
	return _enum;
}

tr::Keycode::operator Scancode() const noexcept
{
	return Scancode::Enum(SDL_GetScancodeFromKey(SDL_Keycode(_enum)));
}

std::string tr::Keycode::name() const noexcept
{
	return SDL_GetKeyName(SDL_Keycode(_enum));
}

bool tr::keyboard::held(Scancode key) noexcept
{
	assert(int(Scancode::Enum(key)) <= int(Scancode::RWIN));
	return SDL_GetKeyboardState(nullptr)[SDL_Scancode(Scancode::Enum(key))];
}

tr::Keymods tr::keyboard::heldMods() noexcept
{
	return Keymods(SDL_GetModState());
}
