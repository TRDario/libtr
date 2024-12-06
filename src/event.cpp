#include "../include/tr/event.hpp"
#include "../include/tr/window.hpp"

#include <SDL2/SDL.h>

using namespace std::chrono_literals;

std::uint32_t tr::generateEventType() noexcept
{
	return SDL_RegisterEvents(1);
}

std::uint32_t tr::Event::type() const noexcept
{
	return ((const SDL_Event*)(&_impl))->type;
}

tr::Event::Event(const CustomEventBase& custom)
{
	auto& sdl{*(SDL_Event*)(&_impl)};

	sdl.type          = custom.type;
	sdl.user.windowID = custom.uint;
	sdl.user.code     = custom.sint;
	sdl.user.data1    = custom.any1.has_value() ? new std::any{std::move(custom.any1)} : nullptr;
	sdl.user.data2    = custom.any2.has_value() ? new std::any{std::move(custom.any2)} : nullptr;
}

tr::Event::operator KeyDownEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::KEY_DOWN);
	return {.repeat = bool(sdl.key.repeat),
			.key{.scan = Scancode::Enum(sdl.key.keysym.scancode),
				 .key  = Keycode::Enum(sdl.key.keysym.sym),
				 .mods = Keymods(sdl.key.keysym.mod)}};
}

tr::Event::operator KeyUpEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::KEY_UP);
	return {.key = {.scan = Scancode::Enum(sdl.key.keysym.scancode),
					.key  = Keycode::Enum(sdl.key.keysym.sym),
					.mods = Keymods(sdl.key.keysym.mod)}};
}

tr::Event::operator TextEditEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::TEXT_EDIT);
	TextEditEvent event{.text = sdl.edit.text};
	event.selected = {event.text.data() + sdl.edit.start, std::size_t(sdl.edit.length)};
	return event;
}

tr::Event::operator TextInputEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::TEXT_INPUT);
	return {.text = sdl.text.text};
}

tr::Event::operator MouseMotionEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::MOUSE_MOTION);
	return {.buttons = MouseButtonMask(sdl.motion.state),
			.pos     = {sdl.motion.x, sdl.motion.y},
			.delta   = {sdl.motion.xrel, sdl.motion.yrel}};
}

tr::Event::operator MouseDownEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::MOUSE_DOWN);
	return {.button = MouseButton(sdl.button.button), .clicks = sdl.button.clicks, .pos = {sdl.button.x, sdl.button.y}};
}

tr::Event::operator MouseUpEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::MOUSE_UP);
	return {.button = MouseButton(sdl.button.button), .pos = {sdl.button.x, sdl.button.y}};
}

tr::Event::operator MouseWheelEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::MOUSE_WHEEL);
	return {.delta = {sdl.wheel.preciseX, sdl.wheel.preciseY}, .mousePos{sdl.wheel.mouseX, sdl.wheel.mouseY}};
}

tr::Event::operator WindowEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::WINDOW);
	switch (sdl.window.event) {
	case SDL_WINDOWEVENT_ENTER:
		return WindowEnterEvent{};
	case SDL_WINDOWEVENT_LEAVE:
		return WindowLeaveEvent{};
	case SDL_WINDOWEVENT_SHOWN:
		return WindowShowEvent{};
	case SDL_WINDOWEVENT_HIDDEN:
		return WindowHideEvent{};
	case SDL_WINDOWEVENT_EXPOSED:
		return WindowExposeEvent{};
	case SDL_WINDOWEVENT_MOVED:
		return WindowMotionEvent{.pos = {sdl.window.data1, sdl.window.data2}};
	case SDL_WINDOWEVENT_RESIZED:
		return WindowResizeEvent{.size = {sdl.window.data1, sdl.window.data2}};
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		return WindowSizeChangeEvent{};
	case SDL_WINDOWEVENT_MINIMIZED:
		return WindowMinimizeEvent{};
	case SDL_WINDOWEVENT_MAXIMIZED:
		return WindowMaximizeEvent{};
	case SDL_WINDOWEVENT_RESTORED:
		return WindowRestoreEvent{};
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		return WindowGainFocusEvent{};
	case SDL_WINDOWEVENT_FOCUS_LOST:
		return WindowLoseFocusEvent{};
	case SDL_WINDOWEVENT_TAKE_FOCUS:
		SDL_SetWindowInputFocus(SDL_GetWindowFromID(sdl.window.windowID));
		break;
	case SDL_WINDOWEVENT_CLOSE:
		return WindowCloseEvent{};
	case SDL_WINDOWEVENT_HIT_TEST:
		return HitTestEvent{};
	}
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
	__assume(false);
#else // GCC, Clang
	__builtin_unreachable();
#endif
}

tr::Event::operator Ticker::Event() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::TICK);
	return {.id = sdl.user.windowID};
}

tr::CustomEventBase tr::Event::getCustomEventBase() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	std::unique_ptr<std::any> any1{(std::any*)(sdl.user.data1)};
	std::unique_ptr<std::any> any2{(std::any*)(sdl.user.data2)};

	return {.type = sdl.type,
			.uint = sdl.user.windowID,
			.sint = sdl.user.code,
			.any1 = any1 != nullptr ? std::move(*any1) : std::any{},
			.any2 = any2 != nullptr ? std::move(*any2) : std::any{}};
}

tr::Ticker::Ticker(std::int32_t id, MillisecondsD interval, std::uint32_t ticks)
	: Ticker{id, interval, ticks, false}
{
}

tr::Ticker::Ticker(std::int32_t id, MillisecondsD interval, std::uint32_t ticks, bool sendDrawEvents)
	: _sendDrawEvents{sendDrawEvents}, _eventID{id}, _ticksLeft{ticks}, _interval{interval}
{
	assert(windowOpened());
	_id = SDL_AddTimer(interval.count(), callback, this);
	if (_id == 0) {
		throw EventError{"Failed to create event ticker"};
	}
}

void tr::Ticker::resetInterval(MillisecondsD interval) noexcept
{
	_interval = interval;
}

std::uint32_t tr::Ticker::callback(std::uint32_t interval, void* ptr) noexcept
{
	auto& self = *(Ticker*)(ptr);
	if (self._sendDrawEvents) {
		window().eventQueue().pushEvent(CustomEventBase{.type = EventType::DRAW});
	}
	else {
		window().eventQueue().pushEvent(CustomEventBase{.type = EventType::TICK, .uint = std::uint32_t(self._eventID)});
	}
	if (self._ticksLeft != TICK_FOREVER && --self._ticksLeft == 0) {
		return 0;
	}
	else {
		self._accumulatedTimerError += self._interval.load() - std::chrono::milliseconds{interval};
		if (self._accumulatedTimerError >= 1ms) {
			self._accumulatedTimerError -= 1ms;
			return std::ceil(self._accumulatedTimerError.count());
		}
		else {
			return std::floor(self._accumulatedTimerError.count());
		}
	}
}

std::optional<tr::Event> tr::EventQueue::pollEvent() noexcept
{
	Event event;
	return SDL_PollEvent((SDL_Event*)(&event._impl)) ? std::optional<Event>{event} : std::nullopt;
}

tr::Event tr::EventQueue::waitForEvent() noexcept
{
	Event event;
	SDL_WaitEvent((SDL_Event*)(&event._impl));
	return event;
}

std::optional<tr::Event> tr::EventQueue::waitForEventTimeout(std::chrono::milliseconds timeout) noexcept
{
	Event event{};
	if (SDL_WaitEventTimeout((SDL_Event*)(&event._impl), timeout.count())) {
		return event;
	}
	else {
		return std::nullopt;
	}
}

void tr::EventQueue::sendDrawEvents(unsigned int frequency)
{
	if (_drawTicker.has_value()) {
		if (frequency == NO_DRAW_EVENTS) {
			_drawTicker.reset();
		}
		else {
			_drawTicker->resetInterval(1000.0ms / frequency);
		}
	}
	else {
		_drawTicker.emplace(0, 1000.0ms / frequency, TICK_FOREVER, true);
	}
}

void tr::EventQueue::sendTextInputEvents(bool arg) noexcept
{
	arg ? SDL_StartTextInput() : SDL_StopTextInput();
}

void tr::EventQueue::pushEvent(const Event& event)
{
	if (SDL_PushEvent((SDL_Event*)(&event)) < 0) {
		throw EventError{"Failed to push event to event queue"};
	}
}
