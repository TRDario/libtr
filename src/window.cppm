module;
#include <magic_enum/magic_enum.hpp>
#include <EnumBitmask.hpp>
#include <SDL2/SDL.h>

export module tr:window;

import std;
import glm;
import :bitmap;
import :display;
import :geometry;
import :bitmap_format;

export namespace tr {
	// Enum representing a window mode.
	enum class WindowMode : std::uint32_t {
		WINDOWED,
		FULLSCREEN = 0x1,
		BORDERLESS = 0x1001
	};
	// Enum representing a window flag.
	enum class WindowFlag : std::uint32_t {
		DEFAULT       = 0x0,
		SHOWN         = SDL_WINDOW_SHOWN,        // The window is visible.
		HIDDEN        = SDL_WINDOW_HIDDEN,       // The window is not visible.
		BORDERLESS    = SDL_WINDOW_BORDERLESS,   // The window has no decoration (topbar, etc.).
		RESIZABLE     = SDL_WINDOW_RESIZABLE,    // The window is resizable.
		MINIMIZED     = SDL_WINDOW_MINIMIZED,    // The window is minimized.
		MAXIMIZED     = SDL_WINDOW_MAXIMIZED,    // The window is maximized.
		GRAB_INPUT    = SDL_WINDOW_INPUT_FOCUS,  // The window has grabbed input focus.
		ALWAYS_ON_TOP = SDL_WINDOW_ALWAYS_ON_TOP // The window is always on top.
	};
	DEFINE_BITMASK_OPERATORS(WindowFlag);
	// Enum representing a flash operation.
	enum class FlashOperation {
		CANCEL,       // Cancel any existing window flashing, if there is any.
		BRIEF,        // Flash the window briefly.
		UNTIL_FOCUSED // Flash the window continuously until the window regains focus.
	};
	// Enum representing a hit test result.
	enum class HitTestResult {
		NORMAL,    // Region is normal and has no special properties.
		DRAGGABLE, // Region can drag entire window.
		TL,        // Region can resize from the top left of the window.
		TOP,       // Region can resize from the top of the window.
		TR,        // Region can resize from the top right of the window.
		RIGHT,     // Region can resize from the right of the window.
		BR,        // Region can resize from the bottom right of the window.
		BOTTOM,    // Region can resize from the bottom of the window.
		BL,        // Region can resize from the bottom left of the window.
		LEFT       // Region can resize from the left of the window.
	};
	// Hit test callback signature.
	using HitTestCB = std::function<HitTestResult(glm::ivec2)>;

	// Value that indicates a centered position on the screen.
	inline constexpr glm::ivec2 CENTERED_POS { 0x2FFF0000, 0x2FFF0000 };

	// Non-owning view over a window.
	class WindowView {
	public:
		explicit WindowView(SDL_Window* window) noexcept;

		// Gets the title of the window.
		const char* title() const noexcept;
		// Sets the title of the window.
		void setTitle(const char* title) const noexcept;

		// Sets the icon of the window.
		void setIcon(const Bitmap& bitmap) const noexcept;

		// Gets the size of the window.
		glm::ivec2 size() const noexcept;
		// Sets the size of the window.
		void setSize(glm::ivec2 size) const noexcept;

		// Gets the window's fullscreen display mode.
		DisplayMode fullscreenMode() const noexcept;
		// Sets the window's fullscreen display mode.
		bool setFullscreenMode(const DisplayMode& dmode) const noexcept;

		// Gets the window's window mode.
		WindowMode windowMode() const noexcept;
		// Sets the window's window mode.
		bool setWindowMode(WindowMode mode) const noexcept;

		// Gets whether the window is resizable.
		bool resizable() const noexcept;
		// Sets whether the window is resizable.
		void setResizable(bool resizable) const noexcept;

		// Gets the minimum resizable size of the window.
		glm::ivec2 minSize() const noexcept;
		// Sets the minimum resizable size of the window.
		void setMinSize(glm::ivec2 minSize) const noexcept;

		// Gets the maximum resizable size of the window.
		glm::ivec2 maxSize() const noexcept;
		// Sets the maximum resizable size of the window.
		void setMaxSize(glm::ivec2 maxSize) const noexcept;

		// Gets the position of the window relative to the top-left corner of the display.
		glm::ivec2 pos() const noexcept;
		// Sets the position of the window relative to the top-left corner of the display.
		void setPos(glm::ivec2 pos) const noexcept;

		// Gets whether the window is bordered or borderless.
		bool bordered() const noexcept;
		// Sets whether the window is bordered or borderless.
		void setBordered(bool bordered) const noexcept;

		// Gets whether the window is being shown.
		bool shown() const noexcept;
		// Unhides the window.
		void show() const noexcept;
		// Hides the window.
		void hide() const noexcept;

		// Gets whether the window is maximized.
		bool maximized() const noexcept;
		// Maximizes the window.
		void maximize() const noexcept;
		// Restores the window back to its previous size.
		void restore() const noexcept;

		// Gets whether the window is minimized.
		bool minimized() const noexcept;
		// Minimizes the window.
		void minimize() const noexcept;

		// Gets whether the window has input focus.
		bool hasFocus() const noexcept;
		// Raises the window above the rest and gives it input focus.
		void raise() const noexcept;

		// Gets whether the mouse is grabbed and confined to this window.
		bool mouseGrabbed() const noexcept;
		// Sets whether the mouse is grabbed and confined to this window.
		void setMouseGrab(bool grab) const noexcept;

		// Gets the area the mouse is confined to when this window has focus, if there is one.
		std::optional<RectI2> mouseConfines() const noexcept;
		// Sets the area the mouse is confined to when this window has focus.
		void setMouseConfines(const RectI2& rect) const noexcept;
		// Resets the area the mouse is confined to.
		void resetMouseConfines() const noexcept;

		// Gets whether the window is always on top.
		bool alwaysOnTop() const noexcept;
		// Sets whether the window is always on top.
		void setAlwaysOnTop(bool alwaysOnTop) const noexcept;

		// Flashes the window to get the user's attention.
		bool flash(FlashOperation operation) const noexcept;

		// Gets the opacity of the window.
		float opacity() const noexcept;
		// Sets the opacity of the window.
		void setOpacity(float opacity) const noexcept;

		// Swaps the display's front and back buffers.
		void swap() const noexcept;
	protected:
		// The underlying SDL window pointer.
		SDL_Window* _impl;

		friend class Event;
	};

	// Window class.
	class Window : public WindowView {
	public:
		// Opens a window.
		Window(const char* title, glm::ivec2 size, glm::ivec2 pos = CENTERED_POS, WindowFlag flags = WindowFlag::DEFAULT);
		// Opens a borderless window.
		Window(const char* title, DisplayInfo display = DEFAULT_DISPLAY);
		// Opens a fullscreen window.
		Window(const char* title, const DisplayMode& dmode, DisplayInfo display = DEFAULT_DISPLAY);
		Window(Window&&) noexcept;
		~Window() noexcept;

		Window& operator=(Window&&) noexcept;

		// Sets a hit-test callback that designates pieces of the window as special.
		bool setHitTest(HitTestCB hitTestCB) noexcept;
	private:
		std::optional<HitTestCB> _hitTest;

		friend class GLContext;
	};
}

export template <>
struct magic_enum::customize::enum_range<tr::WindowMode> {
  static constexpr int min = int(tr::WindowMode::WINDOWED);
  static constexpr int max = int(tr::WindowMode::BORDERLESS);
};

// IMPLEMENTATION

namespace tr {
    // Hit-test callback passed directly to SDL that bridges the gap between SDL and tr's callback format.
    SDL_HitTestResult sdlHitTestCB(SDL_Window* window, const SDL_Point* point, void* data) noexcept;
}

SDL_HitTestResult tr::sdlHitTestCB(SDL_Window* window, const SDL_Point* point, void* data) noexcept
{
    return SDL_HitTestResult((*(HitTestCB*)(data))({ point->x, point->y }));
}

tr::WindowView::WindowView(SDL_Window* window) noexcept
    : _impl { window }
{}

const char* tr::WindowView::title() const noexcept
{
	assert(_impl != nullptr);
    return SDL_GetWindowTitle(_impl);
}

void tr::WindowView::setTitle(const char* title) const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowTitle(_impl, title);
}

void tr::WindowView::setIcon(const Bitmap& bitmap) const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowIcon(_impl, (SDL_Surface*)(bitmap._impl.get()));
}

glm::ivec2 tr::WindowView::size() const noexcept
{
	assert(_impl != nullptr);

    glm::ivec2 size;
    SDL_GetWindowSize(_impl, &size.x, &size.y);
    return size;
}

void tr::WindowView::setSize(glm::ivec2 size) const noexcept
{
	assert(_impl != nullptr);
    assert(size.x > 0 && size.y > 0);
    SDL_SetWindowSize(_impl, size.x, size.y);
}

tr::DisplayMode tr::WindowView::fullscreenMode() const noexcept
{
	assert(_impl != nullptr);
    assert(windowMode() != WindowMode::WINDOWED);
    SDL_DisplayMode sdlMode;
    SDL_GetWindowDisplayMode(_impl, &sdlMode);
    return { { sdlMode.w, sdlMode.h }, BitmapFormat::Type(sdlMode.format), sdlMode.refresh_rate };
}

bool tr::WindowView::setFullscreenMode(const DisplayMode& dmode) const noexcept
{
	assert(_impl != nullptr);
    SDL_DisplayMode sdlMode { std::uint32_t(BitmapFormat::Type(dmode.format)), dmode.size.x, dmode.size.y, dmode.refreshRate };
    return !SDL_SetWindowDisplayMode(_impl, &sdlMode);
}

tr::WindowMode tr::WindowView::windowMode() const noexcept
{
	assert(_impl != nullptr);
    return WindowMode(SDL_GetWindowFlags(_impl) & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP));
}

bool tr::WindowView::setWindowMode(WindowMode mode) const noexcept
{
	assert(_impl != nullptr);
    return !SDL_SetWindowFullscreen(_impl, std::uint32_t(mode));
}

bool tr::WindowView::resizable() const noexcept
{
	assert(_impl != nullptr);
    return SDL_GetWindowFlags(_impl) & SDL_WINDOW_RESIZABLE;
}

void tr::WindowView::setResizable(bool resizable) const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowResizable(_impl, SDL_bool(resizable));
}

glm::ivec2 tr::WindowView::minSize() const noexcept
{
	assert(_impl != nullptr);
    glm::ivec2 minSize;
    SDL_GetWindowMinimumSize(_impl, &minSize.x, &minSize.y);
    return minSize;
}

void tr::WindowView::setMinSize(glm::ivec2 minSize) const noexcept
{
	assert(_impl != nullptr);
    assert(minSize.x > 0 && minSize.y > 0);
    SDL_SetWindowMinimumSize(_impl, minSize.x, minSize.y);
}

glm::ivec2 tr::WindowView::maxSize() const noexcept
{
	assert(_impl != nullptr);
    glm::ivec2 maxSize;
    SDL_GetWindowMaximumSize(_impl, &maxSize.x, &maxSize.y);
    return maxSize;
}

void tr::WindowView::setMaxSize(glm::ivec2 maxSize) const noexcept
{
	assert(_impl != nullptr);
    assert(maxSize.x > 0 && maxSize.y > 0);
    SDL_SetWindowMaximumSize(_impl, maxSize.x, maxSize.y);
}

glm::ivec2 tr::WindowView::pos() const noexcept
{
	assert(_impl != nullptr);
    glm::ivec2 pos;
    SDL_GetWindowPosition(_impl, &pos.x, &pos.y);
    return pos;
}

void tr::WindowView::setPos(glm::ivec2 pos) const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowPosition(_impl, pos.x, pos.y);
}

bool tr::WindowView::bordered() const noexcept
{
	assert(_impl != nullptr);
    return !(SDL_GetWindowFlags(_impl) & SDL_WINDOW_BORDERLESS);
}

void tr::WindowView::setBordered(bool bordered) const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowBordered(_impl, SDL_bool(bordered));
}

bool tr::WindowView::shown() const noexcept
{
	assert(_impl != nullptr);
    return SDL_GetWindowFlags(_impl) & SDL_WINDOW_SHOWN;
}

void tr::WindowView::show() const noexcept
{
	assert(_impl != nullptr);
    SDL_ShowWindow(_impl);
}

void tr::WindowView::hide() const noexcept
{
	assert(_impl != nullptr);
    SDL_HideWindow(_impl);
}

bool tr::WindowView::maximized() const noexcept
{
	assert(_impl != nullptr);
    return SDL_GetWindowFlags(_impl) & SDL_WINDOW_MAXIMIZED;
}

void tr::WindowView::maximize() const noexcept
{
	assert(_impl != nullptr);
    SDL_MaximizeWindow(_impl);
}

void tr::WindowView::restore() const noexcept
{
	assert(_impl != nullptr);
    SDL_RestoreWindow(_impl);
}

bool tr::WindowView::minimized() const noexcept
{
	assert(_impl != nullptr);
    return SDL_GetWindowFlags(_impl) & SDL_WINDOW_MINIMIZED;
}

void tr::WindowView::minimize() const noexcept
{
	assert(_impl != nullptr);
    SDL_MinimizeWindow(_impl);
}

bool tr::WindowView::hasFocus() const noexcept
{
	assert(_impl != nullptr);
    return SDL_GetWindowFlags(_impl) & SDL_WINDOW_INPUT_FOCUS;
}

void tr::WindowView::raise() const noexcept
{
	assert(_impl != nullptr);
    SDL_RaiseWindow(_impl);
}

bool tr::WindowView::mouseGrabbed() const noexcept
{
	assert(_impl != nullptr);
    return SDL_GetWindowGrab(_impl);
}

void tr::WindowView::setMouseGrab(bool grab) const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowGrab(_impl, SDL_bool(grab));
}

std::optional<tr::RectI2> tr::WindowView::mouseConfines() const noexcept
{
	assert(_impl != nullptr);
    auto sdlRect { SDL_GetWindowMouseRect(_impl) };
    if (sdlRect == nullptr) {
        return std::nullopt;
    }
    else {
        return std::bit_cast<RectI2>(*sdlRect);
    }
}

void tr::WindowView::setMouseConfines(const RectI2& rect) const noexcept
{
	assert(_impl != nullptr);
    auto sdlRect { std::bit_cast<SDL_Rect>(rect) };
    SDL_SetWindowMouseRect(_impl, &sdlRect);
}

void tr::WindowView::resetMouseConfines() const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowMouseRect(_impl, nullptr);
}

bool tr::WindowView::alwaysOnTop() const noexcept
{
	assert(_impl != nullptr);
    return SDL_GetWindowFlags(_impl) & SDL_WINDOW_ALWAYS_ON_TOP;
}

void tr::WindowView::setAlwaysOnTop(bool alwaysOnTop) const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowAlwaysOnTop(_impl, SDL_bool(alwaysOnTop));
}

bool tr::WindowView::flash(FlashOperation operation) const noexcept
{
	assert(_impl != nullptr);
    return !SDL_FlashWindow(_impl, SDL_FlashOperation(operation));
}

float tr::WindowView::opacity() const noexcept
{
	assert(_impl != nullptr);
    float opacity;
    SDL_GetWindowOpacity(_impl, &opacity);
    return opacity;
}

void tr::WindowView::setOpacity(float opacity) const noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowOpacity(_impl, opacity);
}

void tr::WindowView::swap() const noexcept
{
	assert(_impl != nullptr);
    SDL_GL_SwapWindow(_impl);
}

tr::Window::Window(const char* title, glm::ivec2 size, glm::ivec2 pos, WindowFlag flags)
    : WindowView { SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, std::uint32_t(flags) | SDL_WINDOW_OPENGL) }
{
    if (_impl == nullptr) {
        throw std::runtime_error { std::format("Failed to open {}x{} window ({}).", size.x, size.y, SDL_GetError()) };
    }
}

tr::Window::Window(const char* title, DisplayInfo display)
    : WindowView { nullptr }
{
    const auto dmode { display.desktopMode() };
    const auto pos { display.centeredPos() };
    _impl = SDL_CreateWindow(title, pos.x, pos.y, dmode.size.x, dmode.size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (_impl == nullptr) {
        throw std::runtime_error { std::format("Failed to open borderless {}x{} window ({}).", dmode.size.x, dmode.size.y, SDL_GetError()) };
    }
}

tr::Window::Window(const char* title, const DisplayMode& dmode, DisplayInfo display)
    : WindowView { nullptr }
{
    auto pos { display.centeredPos() };
    _impl = SDL_CreateWindow(title, pos.x, pos.y, dmode.size.x, dmode.size.y, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    if (_impl == nullptr || !setFullscreenMode(dmode)) {
        throw std::runtime_error { std::format("Failed to open fullscreen {}x{} window ({}).", dmode.size.x, dmode.size.y, SDL_GetError()) };
    }
}

tr::Window::Window(Window&& r) noexcept
    : WindowView { r._impl }
    , _hitTest { std::move(r._hitTest) }
{
    r._impl = nullptr;
}

tr::Window::~Window() noexcept
{
    SDL_DestroyWindow(_impl);
}

tr::Window& tr::Window::operator=(Window&& r) noexcept
{
    std::ignore = std::move(*this);
    std::swap(_impl, r._impl);
    std::swap(_hitTest, r._hitTest);
    return *this;
}

bool tr::Window::setHitTest(HitTestCB hitTestCB) noexcept
{
	assert(_impl != nullptr);
    SDL_SetWindowHitTest(_impl, nullptr, nullptr);
    if (hitTestCB != nullptr) {
        _hitTest = std::move(hitTestCB);
        return !SDL_SetWindowHitTest(_impl, sdlHitTestCB, &_hitTest);
    }
    return true;
}