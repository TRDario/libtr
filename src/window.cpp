#include "../include/tr/window.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <format>

namespace tr {
	Window* _window{nullptr};

	bool        initSDL(const GraphicsProperties& gfxProperties);
	void        setSDLGLAttributes(const GraphicsProperties& gfxProperties) noexcept;
	void        suppressUnsupportedEvents() noexcept;
	SDL_Window* openWindowedWindow(const char* title, glm::ivec2 size, glm::ivec2 pos, WindowFlag flags);
	SDL_Window* openBorderlessWindow(const char* title, DisplayInfo display, WindowFlag flags);
	SDL_Window* openFullscreenWindow(const char* title, const DisplayMode& dmode, DisplayInfo display,
									 WindowFlag flags);
} // namespace tr

void tr::setSDLGLAttributes(const GraphicsProperties& gfxProperties) noexcept
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG |
												  (gfxProperties.debugContext ? SDL_GL_CONTEXT_DEBUG_FLAG : 0));
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, gfxProperties.depthBits);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, gfxProperties.stencilBits);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, bool(gfxProperties.multisamples));
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, gfxProperties.multisamples);
}

void tr::suppressUnsupportedEvents() noexcept
{
	SDL_EventState(SDL_AUDIODEVICEADDED, SDL_IGNORE);
	SDL_EventState(SDL_AUDIODEVICEREMOVED, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_AUDIODEVICEREMOVED, SDL_IGNORE);
	SDL_EventState(SDL_DOLLARGESTURE, SDL_IGNORE);
	SDL_EventState(SDL_DOLLARRECORD, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERDEVICEADDED, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERDEVICEREMOVED, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERDEVICEREMAPPED, SDL_IGNORE);
	SDL_EventState(SDL_DROPFILE, SDL_IGNORE);
	SDL_EventState(SDL_DROPTEXT, SDL_IGNORE);
	SDL_EventState(SDL_DROPBEGIN, SDL_IGNORE);
	SDL_EventState(SDL_DROPCOMPLETE, SDL_IGNORE);
	SDL_EventState(SDL_FINGERMOTION, SDL_IGNORE);
	SDL_EventState(SDL_FINGERDOWN, SDL_IGNORE);
	SDL_EventState(SDL_FINGERUP, SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_JOYDEVICEADDED, SDL_IGNORE);
	SDL_EventState(SDL_JOYDEVICEREMOVED, SDL_IGNORE);
	SDL_EventState(SDL_MULTIGESTURE, SDL_IGNORE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_StopTextInput();
}

bool tr::initSDL(const GraphicsProperties& gfxProperties)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
		SDL_Quit();
		throw WindowOpenError{"Failed to initialize SDL"};
	}
	setSDLGLAttributes(gfxProperties);
	suppressUnsupportedEvents();
	return true;
}

SDL_Window* tr::openWindowedWindow(const char* title, glm::ivec2 size, glm::ivec2 pos, WindowFlag flags)
{
	const auto  sdlFlags{std::uint32_t(flags) | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN};
	SDL_Window* window{SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, sdlFlags)};
	if (window == nullptr) {
		throw WindowOpenError{std::format("Failed to open {}x{} window ({}).", size.x, size.y, SDL_GetError())};
	}
	return window;
}

SDL_Window* tr::openBorderlessWindow(const char* title, DisplayInfo display, WindowFlag flags)
{
	const auto  size{display.displayMode(DESKTOP_MODE).size};
	const auto  pos{display.centeredPos()};
	const auto  sdlFlags{std::uint32_t(flags) | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_HIDDEN};
	SDL_Window* window{SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, sdlFlags)};
	if (window == nullptr) {
		throw WindowOpenError{
			std::format("Failed to open borderless {}x{} window ({}).", size.x, size.y, SDL_GetError())};
	}
	return window;
}

SDL_Window* tr::openFullscreenWindow(const char* title, const DisplayMode& dmode, DisplayInfo display, WindowFlag flags)
{
	auto            pos{display.centeredPos()};
	SDL_DisplayMode sdlMode{std::uint32_t(BitmapFormat::Type(dmode.format)), dmode.size.x, dmode.size.y,
							dmode.refreshRate, nullptr};
	const auto      sdlFlags{std::uint32_t(flags) | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN};
	SDL_Window*     window{SDL_CreateWindow(title, pos.x, pos.y, dmode.size.x, dmode.size.y, sdlFlags)};
	if (window == nullptr || SDL_SetWindowDisplayMode(window, &sdlMode) < 0) {
		throw WindowOpenError{
			std::format("Failed to open fullscreen {}x{} window ({}).", dmode.size.x, dmode.size.y, SDL_GetError())};
	}
	return window;
}

tr::Window::Window(const char* title, glm::ivec2 size, glm::ivec2 pos, WindowFlag flags,
				   const GraphicsProperties& gfxProperties)
	: _sdl{initSDL(gfxProperties)}, _impl{openWindowedWindow(title, size, pos, flags)}, _glContext{_impl.get()}
{
	assert(!windowOpened());
	_window = this;
}

tr::Window::Window(const char* title, DisplayInfo display, WindowFlag flags, const GraphicsProperties& gfxProperties)
	: _sdl{initSDL(gfxProperties)}, _impl{openBorderlessWindow(title, display, flags)}, _glContext{_impl.get()}
{
	assert(!windowOpened());
	_window = this;
}

tr::Window::Window(const char* title, const DisplayMode& dmode, DisplayInfo display, WindowFlag flags,
				   const GraphicsProperties& gfxProperties)
	: _sdl{initSDL(gfxProperties)}, _impl{openFullscreenWindow(title, dmode, display, flags)}, _glContext{_impl.get()}
{
	assert(!windowOpened());
	_window = this;
}

void tr::Window::SDLDeleter::operator()(bool) const noexcept
{
	if (TTF_WasInit()) {
		TTF_Quit();
	}
	if (IMG_Init(0)) {
		IMG_Quit();
	}
	if (SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		SDL_Quit();
	}
}

void tr::Window::WindowDeleter::operator()(SDL_Window* ptr) const noexcept
{
	SDL_DestroyWindow(ptr);
	_window = nullptr; // Must be placed this late to make sure some other components get destroyed.
}

const char* tr::Window::title() const noexcept
{
	assert(_impl.get() != nullptr);
	return SDL_GetWindowTitle(_impl.get());
}

void tr::Window::setTitle(const char* title) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowTitle(_impl.get(), title);
}

void tr::Window::setTitle(const std::string& title) noexcept
{
	setTitle(title.c_str());
}

void tr::Window::setIcon(const Bitmap& bitmap) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowIcon(_impl.get(), bitmap._impl.get());
}

void tr::Window::setIcon(const BitmapView& view) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowIcon(_impl.get(), view._impl.get());
}

glm::ivec2 tr::Window::size() const noexcept
{
	assert(_impl.get() != nullptr);

	glm::ivec2 size;
	SDL_GetWindowSize(_impl.get(), &size.x, &size.y);
	return size;
}

void tr::Window::setSize(glm::ivec2 size) noexcept
{
	assert(_impl.get() != nullptr);
	assert(size.x > 0 && size.y > 0);
	SDL_SetWindowSize(_impl.get(), size.x, size.y);
}

std::optional<tr::DisplayMode> tr::Window::fullscreenMode() const noexcept
{
	assert(_impl.get() != nullptr);

	if (windowMode() == WindowMode::WINDOWED) {
		return std::nullopt;
	}
	else {
		SDL_DisplayMode sdlMode;
		SDL_GetWindowDisplayMode(_impl.get(), &sdlMode);
		return DisplayMode{{sdlMode.w, sdlMode.h}, BitmapFormat::Type(sdlMode.format), sdlMode.refresh_rate};
	}
}

void tr::Window::setFullscreenMode(const DisplayMode& dmode)
{
	assert(_impl.get() != nullptr);
	SDL_DisplayMode sdlMode{std::uint32_t(BitmapFormat::Type(dmode.format)), dmode.size.x, dmode.size.y,
							dmode.refreshRate, nullptr};
	if (SDL_SetWindowDisplayMode(_impl.get(), &sdlMode) < 0) {
		throw WindowError{"Failed to set window fullscreen mode"};
	}
}

tr::WindowMode tr::Window::windowMode() const noexcept
{
	assert(_impl.get() != nullptr);
	return WindowMode(SDL_GetWindowFlags(_impl.get()) & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP));
}

void tr::Window::setWindowMode(WindowMode mode)
{
	assert(_impl.get() != nullptr);
	if (SDL_SetWindowFullscreen(_impl.get(), std::uint32_t(mode)) < 0) {
		throw WindowError{"Failed to set window mode"};
	}
}

bool tr::Window::resizable() const noexcept
{
	assert(_impl.get() != nullptr);
	return SDL_GetWindowFlags(_impl.get()) & SDL_WINDOW_RESIZABLE;
}

void tr::Window::setResizable(bool resizable) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowResizable(_impl.get(), SDL_bool(resizable));
}

glm::ivec2 tr::Window::minSize() const noexcept
{
	assert(_impl.get() != nullptr);
	glm::ivec2 minSize;
	SDL_GetWindowMinimumSize(_impl.get(), &minSize.x, &minSize.y);
	return minSize;
}

void tr::Window::setMinSize(glm::ivec2 minSize) noexcept
{
	assert(_impl.get() != nullptr);
	assert(minSize.x > 0 && minSize.y > 0);
	SDL_SetWindowMinimumSize(_impl.get(), minSize.x, minSize.y);
}

glm::ivec2 tr::Window::maxSize() const noexcept
{
	assert(_impl.get() != nullptr);
	glm::ivec2 maxSize;
	SDL_GetWindowMaximumSize(_impl.get(), &maxSize.x, &maxSize.y);
	return maxSize;
}

void tr::Window::setMaxSize(glm::ivec2 maxSize) noexcept
{
	assert(_impl.get() != nullptr);
	assert(maxSize.x > 0 && maxSize.y > 0);
	SDL_SetWindowMaximumSize(_impl.get(), maxSize.x, maxSize.y);
}

glm::ivec2 tr::Window::position() const noexcept
{
	assert(_impl.get() != nullptr);
	glm::ivec2 pos;
	SDL_GetWindowPosition(_impl.get(), &pos.x, &pos.y);
	return pos;
}

void tr::Window::setPosition(glm::ivec2 pos) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowPosition(_impl.get(), pos.x, pos.y);
}

bool tr::Window::bordered() const noexcept
{
	assert(_impl.get() != nullptr);
	return !(SDL_GetWindowFlags(_impl.get()) & SDL_WINDOW_BORDERLESS);
}

void tr::Window::setBordered(bool bordered) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowBordered(_impl.get(), SDL_bool(bordered));
}

bool tr::Window::shown() const noexcept
{
	assert(_impl.get() != nullptr);
	return SDL_GetWindowFlags(_impl.get()) & SDL_WINDOW_SHOWN;
}

void tr::Window::show() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_ShowWindow(_impl.get());
}

void tr::Window::hide() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_HideWindow(_impl.get());
}

bool tr::Window::maximized() const noexcept
{
	assert(_impl.get() != nullptr);
	return SDL_GetWindowFlags(_impl.get()) & SDL_WINDOW_MAXIMIZED;
}

void tr::Window::maximize() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_MaximizeWindow(_impl.get());
}

void tr::Window::restore() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_RestoreWindow(_impl.get());
}

bool tr::Window::minimized() const noexcept
{
	assert(_impl.get() != nullptr);
	return SDL_GetWindowFlags(_impl.get()) & SDL_WINDOW_MINIMIZED;
}

void tr::Window::minimize() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_MinimizeWindow(_impl.get());
}

bool tr::Window::hasFocus() const noexcept
{
	assert(_impl.get() != nullptr);
	return SDL_GetWindowFlags(_impl.get()) & SDL_WINDOW_INPUT_FOCUS;
}

void tr::Window::raise() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_RaiseWindow(_impl.get());
}

bool tr::Window::mouseGrabbed() const noexcept
{
	assert(_impl.get() != nullptr);
	return SDL_GetWindowGrab(_impl.get());
}

void tr::Window::setMouseGrab(bool grab) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowGrab(_impl.get(), SDL_bool(grab));
}

std::optional<tr::RectI2> tr::Window::mouseConfines() const noexcept
{
	assert(_impl.get() != nullptr);
	auto sdlRect{SDL_GetWindowMouseRect(_impl.get())};
	if (sdlRect == nullptr) {
		return std::nullopt;
	}
	else {
		return std::bit_cast<RectI2>(*sdlRect);
	}
}

void tr::Window::setMouseConfines(const RectI2& rect) noexcept
{
	assert(_impl.get() != nullptr);
	auto sdlRect{std::bit_cast<SDL_Rect>(rect)};
	SDL_SetWindowMouseRect(_impl.get(), &sdlRect);
}

void tr::Window::resetMouseConfines() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowMouseRect(_impl.get(), nullptr);
}

bool tr::Window::alwaysOnTop() const noexcept
{
	assert(_impl.get() != nullptr);
	return SDL_GetWindowFlags(_impl.get()) & SDL_WINDOW_ALWAYS_ON_TOP;
}

void tr::Window::setAlwaysOnTop(bool alwaysOnTop) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowAlwaysOnTop(_impl.get(), SDL_bool(alwaysOnTop));
}

void tr::Window::flash(FlashOperation operation) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_FlashWindow(_impl.get(), SDL_FlashOperation(operation));
}

float tr::Window::opacity() const noexcept
{
	assert(_impl.get() != nullptr);
	float opacity;
	SDL_GetWindowOpacity(_impl.get(), &opacity);
	return opacity;
}

void tr::Window::setOpacity(float opacity) noexcept
{
	assert(_impl.get() != nullptr);
	SDL_SetWindowOpacity(_impl.get(), opacity);
}

tr::VSync tr::Window::vsync() const noexcept
{
	return VSync(SDL_GL_GetSwapInterval());
}

void tr::Window::setVSync(VSync vsync) noexcept
{
	if (SDL_GL_SetSwapInterval(int(vsync)) < 0 && vsync == VSync::ADAPTIVE) {
		setVSync(VSync::ENABLED);
	}
}

tr::GraphicsContext& tr::Window::graphics() noexcept
{
	return _glContext;
}

const tr::GraphicsContext& tr::Window::graphics() const noexcept
{
	return _glContext;
}

tr::EventQueue& tr::Window::events() noexcept
{
	return _eventQueue;
}

const tr::EventQueue& tr::Window::events() const noexcept
{
	return _eventQueue;
}

tr::Backbuffer& tr::Window::backbuffer() noexcept
{
	return _backbuffer;
}

const tr::Backbuffer& tr::Window::backbuffer() const noexcept
{
	return _backbuffer;
}

const tr::Keyboard& tr::Window::keyboard() const noexcept
{
	return _keyboard;
}

tr::Mouse& tr::Window::mouse() noexcept
{
	return _mouse;
}

const tr::Mouse& tr::Window::mouse() const noexcept
{
	return _mouse;
}

bool tr::windowOpened() noexcept
{
	return _window != nullptr;
}

tr::Window& tr::window() noexcept
{
	assert(windowOpened());
	return *_window;
}