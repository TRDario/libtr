#include "../include/window.hpp"
#include <SDL2/SDL.h>


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

std::optional<tr::DisplayMode> tr::WindowView::fullscreenMode() const noexcept
{
	assert(_impl != nullptr);

	if (windowMode() == WindowMode::WINDOWED) {
		return std::nullopt;
	}
	else {
		SDL_DisplayMode sdlMode;
		SDL_GetWindowDisplayMode(_impl, &sdlMode);
		return DisplayMode { { sdlMode.w, sdlMode.h }, BitmapFormat::Type(sdlMode.format), sdlMode.refresh_rate };
	}
}

void tr::WindowView::setFullscreenMode(const DisplayMode& dmode) const
{
	assert(_impl != nullptr);
	SDL_DisplayMode sdlMode { std::uint32_t(BitmapFormat::Type(dmode.format)), dmode.size.x, dmode.size.y, dmode.refreshRate };
	if (SDL_SetWindowDisplayMode(_impl, &sdlMode) < 0) {
		throw WindowError { "Failed to set window fullscreen mode" };
	}
}

tr::WindowMode tr::WindowView::windowMode() const noexcept
{
	assert(_impl != nullptr);
	return WindowMode(SDL_GetWindowFlags(_impl) & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP));
}

void tr::WindowView::setWindowMode(WindowMode mode) const
{
	assert(_impl != nullptr);
	if (SDL_SetWindowFullscreen(_impl, std::uint32_t(mode)) < 0) {
		throw WindowError { "Failed to set window mode" };
	}
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

void tr::WindowView::flash(FlashOperation operation) const
{
	assert(_impl != nullptr);
	if (SDL_FlashWindow(_impl, SDL_FlashOperation(operation)) < 0) {
		throw WindowError { "Failed to flash window" };
	}
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
		throw WindowOpenError { std::format("Failed to open {}x{} window ({}).", size.x, size.y, SDL_GetError()) };
	}
}

tr::Window::Window(const char* title, DisplayInfo display, WindowFlag flags)
	: WindowView { nullptr }
{
	const auto dmode { display.desktopMode() };
	const auto pos { display.centeredPos() };
	_impl = SDL_CreateWindow(title, pos.x, pos.y, dmode.size.x, dmode.size.y, std::uint32_t(flags) | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (_impl == nullptr) {
		throw WindowOpenError { std::format("Failed to open borderless {}x{} window ({}).", dmode.size.x, dmode.size.y, SDL_GetError()) };
	}
}

tr::Window::Window(const char* title, const DisplayMode& dmode, DisplayInfo display, WindowFlag flags)
	: WindowView { nullptr }
{
	auto pos { display.centeredPos() };
	_impl = SDL_CreateWindow(title, pos.x, pos.y, dmode.size.x, dmode.size.y, std::uint32_t(flags) | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	if (_impl == nullptr) {
		throw WindowOpenError { std::format("Failed to open fullscreen {}x{} window ({}).", dmode.size.x, dmode.size.y, SDL_GetError()) };
	}
	setFullscreenMode(dmode);
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

void tr::Window::setHitTest(HitTestCB hitTestCB)
{
	assert(_impl != nullptr);
	SDL_SetWindowHitTest(_impl, nullptr, nullptr);
	if (hitTestCB != nullptr) {
		auto oldHitTest { std::exchange(_hitTest, std::move(hitTestCB)) };
		if (SDL_SetWindowHitTest(_impl, sdlHitTestCB, &_hitTest) < 0) {
			std::swap(oldHitTest, _hitTest);
			throw WindowError { "Failed to set hit test" };
		}
	}
}