module;
#include <cassert>
#include <EnumBitmask.hpp>
#include <SDL2/SDL.h>

export module tr:keyboard;

import std;

export namespace tr {
	class Scancode;
	class Keycode;
	enum class Keymods : std::uint16_t;

	// Keyboard functions.
	namespace keyboard {
		// Gets whether a physical keyboard key is held.
		bool held(Scancode key) noexcept;
		// Gets a bitmask of the held keyboard modifiers.
		Keymods heldMods() noexcept;
	}

	// Physical key code class.
	class Scancode {
	public:
		enum class Enum {
			UNKNOWN,
			A = 4, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			TOP_ROW_1, TOP_ROW_2, TOP_ROW_3, TOP_ROW_4, TOP_ROW_5, TOP_ROW_6, TOP_ROW_7, TOP_ROW_8, TOP_ROW_9, TOP_ROW_0,
			ENTER,
			ESCAPE,
			BACKSPACE,
			TAB,
			SPACE,
			MINUS,
			EQUALS,
			LEFT_BRACKET, RIGHT_BRACKET,
			BACKSLASH,
			SEMICOLON = 51,
			APOSTROPHE,
			GRAVE,
			COMMA,
			PERIOD,
			SLASH,
			CAPSLOCK,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			PRINT_SCREEN,
			SCROLL_LOCK,
			PAUSE,
			INSERT,
			HOME,
			PAGE_UP,
			DELETE,
			END,
			PAGE_DOWN,
			RIGHT, LEFT, DOWN, UP,
			NUM_LOCK,
			KEYPAD_DIVIDE, KEYPAD_MULTIPLY, KEYPAD_MINUS, KEYPAD_PLUS, KEYPAD_ENTER,
			KEYPAD_1, KEYPAD_2, KEYPAD_3, KEYPAD_4, KEYPAD_5, KEYPAD_6, KEYPAD_7, KEYPAD_8, KEYPAD_9, KEYPAD_0,
			KEYPAD_PERIOD,
			NONUS_BACKSLASH,
			APPLICATION,
			LCTRL = 224,
			LSHIFT,
			LALT,
			LWIN,
			RCTRL,
			RSHIFT,
			RALT,
			RWIN
		};
		using enum Enum;

		Scancode(Enum base) noexcept;
		Scancode(const char* name) noexcept;

		operator Enum	() const noexcept;
		operator Keycode() const noexcept;
		const char* name() const noexcept;
	private:
		Enum _enum;
	};

	// Virtual key code class.
	class Keycode {
	public:
		enum class Enum {
			UNKNOWN,
			BACKSPACE = '\b',
			TAB,
			ENTER = '\r',
			ESCAPE = '\x1b',
			SPACE = ' ',
			EXCLAMATION_MARK,
			DOUBLE_QUOTE,
			HASH,
			DOLLAR,
			PERCENT,
			AMPERSAND,
			QUOTE,
			LEFT_PARENTHESIS, RIGHT_PARENTHESIS,
			ASTERRISK,
			PLUS,
			COMMA,
			MINUS,
			PERIOD,
			SLASH,
			TOP_ROW_1, TOP_ROW_2, TOP_ROW_3, TOP_ROW_4, TOP_ROW_5, TOP_ROW_6, TOP_ROW_7, TOP_ROW_8, TOP_ROW_9, TOP_ROW_0,
			COLON,
			SEMICOLON,
			LESS,
			EQUALS,
			GREATER,
			QUESTION_MARK,
			AT,
			LEFT_BRACKET = '[',
			BACKSLASH,
			RIGHT_BRACKET,
			CARET,
			UNDERSCORE,
			BACKQUOTE,
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			CAPSLOCK = 1073741881,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			PRINT_SCREEN,
			SCROLL_LOCK,
			PAUSE,
			INSERT,
			HOME,
			PAGE_UP,
			DELETE,
			END,
			PAGE_DOWN,
			RIGHT, LEFT, DOWN, UP,
			NUM_LOCK,
			KEYPAD_DIVIDE, KEYPAD_MULTIPLY, KEYPAD_MINUS, KEYPAD_PLUS, KEYPAD_ENTER,
			KEYPAD_1, KEYPAD_2, KEYPAD_3, KEYPAD_4, KEYPAD_5, KEYPAD_6, KEYPAD_7, KEYPAD_8, KEYPAD_9, KEYPAD_0,
			KEYPAD_PERIOD,
			APPLICATION = 1073741925,
			LCTRL = 1073742048,
			LSHIFT,
			LALT,
			LWIN,
			RCTRL,
			RSHIFT,
			RALT,
			RWIN
		};
		using enum Enum;

		Keycode(Enum base) noexcept;
		Keycode(const char* name) noexcept;

		operator Enum	 () const noexcept;
		operator Scancode() const noexcept;
		std::string name() const noexcept;
	private:
		Enum _enum;
	};

	// Enum representing a key modifier.
	enum class Keymods : std::uint16_t {
		NONE = 0x0000,
		LSHIFT = 0x0001,
		RSHIFT = 0x0002,
		LCTRL = 0x0040,
		RCTRL = 0x0080,
		LALT = 0x0100,
		RALT = 0x0200,
		LWIN = 0x0400,
		RWIN = 0x0800,
		NUM = 0x1000,
		CAPS = 0x2000,
		MODE = 0x4000,
		SCROLL = 0x8000
	};
	DEFINE_BITMASK_OPERATORS(Keymods);
	
	// Structure containing key information in key events.
	struct KeyInfo {
		Scancode scan;
		Keycode  key;
		Keymods  mods;
	};
}

// IMPLEMENTATION

tr::Scancode::Scancode(Enum base) noexcept
    : _enum { base }
{}

tr::Scancode::Scancode(const char* name) noexcept
    : _enum { Enum(SDL_GetScancodeFromName(name)) }
{}

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
    : _enum { base }
{}

tr::Keycode::Keycode(const char* name) noexcept
    : _enum { Enum(SDL_GetKeyFromName(name)) }
{}

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