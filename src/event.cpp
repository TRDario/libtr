#include "../include/tr/event.hpp"

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
	return {.win    = WindowView{SDL_GetWindowFromID(sdl.key.windowID)},
			.repeat = bool(sdl.key.repeat),
			.key{.scan = Scancode::Enum(sdl.key.keysym.scancode),
				 .key  = Keycode::Enum(sdl.key.keysym.sym),
				 .mods = Keymods(sdl.key.keysym.mod)}};
}

tr::Event::operator KeyUpEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::KEY_UP);
	return {.win = WindowView{SDL_GetWindowFromID(sdl.key.windowID)},
			.key = {.scan = Scancode::Enum(sdl.key.keysym.scancode),
					.key  = Keycode::Enum(sdl.key.keysym.sym),
					.mods = Keymods(sdl.key.keysym.mod)}};
}

tr::Event::operator TextEditEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::TEXT_EDIT);
	TextEditEvent event{.win = WindowView{SDL_GetWindowFromID(sdl.edit.windowID)}, .text = sdl.edit.text};
	event.selected = {event.text.data() + sdl.edit.start, std::size_t(sdl.edit.length)};
	return event;
}

tr::Event::operator TextInputEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::TEXT_INPUT);
	return {.win = WindowView{SDL_GetWindowFromID(sdl.text.windowID)}, .text = sdl.text.text};
}

tr::Event::operator MouseMotionEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::MOUSE_MOTION);
	return {.win     = WindowView{SDL_GetWindowFromID(sdl.motion.windowID)},
			.buttons = MouseButtonMask(sdl.motion.state),
			.pos     = {sdl.motion.x, sdl.motion.y},
			.delta   = {sdl.motion.xrel, sdl.motion.yrel}};
}

tr::Event::operator MouseDownEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::MOUSE_DOWN);
	return {.win    = WindowView{SDL_GetWindowFromID(sdl.button.windowID)},
			.button = MouseButton(sdl.button.button),
			.clicks = sdl.button.clicks,
			.pos    = {sdl.button.x, sdl.button.y}};
}

tr::Event::operator MouseUpEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::MOUSE_UP);
	return {.win    = WindowView{SDL_GetWindowFromID(sdl.button.windowID)},
			.button = MouseButton(sdl.button.button),
			.pos    = {sdl.button.x, sdl.button.y}};
}

tr::Event::operator MouseWheelEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::MOUSE_WHEEL);
	return {.win   = WindowView{SDL_GetWindowFromID(sdl.wheel.windowID)},
			.delta = {sdl.wheel.preciseX, sdl.wheel.preciseY},
			.mousePos{sdl.wheel.mouseX, sdl.wheel.mouseY}};
}

tr::Event::operator WindowEvent() const noexcept
{
	auto& sdl{*(const SDL_Event*)(&_impl)};

	assert(type() == EventType::WINDOW);
	switch (sdl.window.event) {
	case SDL_WINDOWEVENT_ENTER:
		return WindowEnterEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_LEAVE:
		return WindowLeaveEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_SHOWN:
		return WindowShowEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_HIDDEN:
		return WindowHideEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_EXPOSED:
		return WindowExposeEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_MOVED:
		return WindowMotionEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)},
								 .pos = {sdl.window.data1, sdl.window.data2}};
	case SDL_WINDOWEVENT_RESIZED:
		return WindowResizeEvent{.win  = WindowView{SDL_GetWindowFromID(sdl.window.windowID)},
								 .size = {sdl.window.data1, sdl.window.data2}};
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		return WindowSizeChangeEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_MINIMIZED:
		return WindowMinimizeEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_MAXIMIZED:
		return WindowMaximizeEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_RESTORED:
		return WindowRestoreEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		return WindowGainFocusEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_FOCUS_LOST:
		return WindowLoseFocusEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_TAKE_FOCUS:
		SDL_SetWindowInputFocus(SDL_GetWindowFromID(sdl.window.windowID));
		break;
	case SDL_WINDOWEVENT_CLOSE:
		return WindowCloseEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	case SDL_WINDOWEVENT_HIT_TEST:
		return HitTestEvent{.win = WindowView{SDL_GetWindowFromID(sdl.window.windowID)}};
	}
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
	__assume(false);
#else // GCC, Clang
	__builtin_unreachable();
#endif
}

tr::Event::operator TickEvent() const noexcept
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

tr::EventQueue::~EventQueue() noexcept
{
	if (_drawTicker != NO_DRAW_EVENTS) {
		SDL_RemoveTimer(_drawTicker);
		_tickerData.erase(_drawTicker);
	}
	for (auto& ticker : std::views::keys(_tickerData)) {
		SDL_RemoveTimer(ticker);
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

tr::Ticker tr::EventQueue::addTicker(std::int32_t id, MillisecondsD interval, std::uint32_t nticks)
{
	auto tickerData{std::make_unique<TickerData>(std::ref(*this), id, interval, 0.0ms, nticks)};
	auto ticker{SDL_AddTimer(interval.count(), tickerCallback, tickerData.get())};
	if (ticker == 0) {
		throw SDLError{"Failed to add event ticker to event queue"};
	}
	_tickerData.emplace(ticker, std::move(tickerData));
	return Ticker{ticker};
}

void tr::EventQueue::removeTicker(Ticker ticker) noexcept
{
	SDL_RemoveTimer(SDL_TimerID(ticker));
	_tickerData.erase(std::uint32_t(ticker));
}

std::uint32_t tr::EventQueue::tickerCallback(std::uint32_t interval, void* ptr) noexcept
{
	auto& data{*(TickerData*)(ptr)};
	data.queue.pushEvent(CustomEventBase{.type = EventType::TICK, .uint = std::uint32_t(data.id)});
	if (data.ticksLeft != TICK_FOREVER && --data.ticksLeft == 0) {
		return 0;
	}
	else {
		data.accumulatedError += data.preciseInterval - std::chrono::milliseconds{interval};
		if (data.accumulatedError >= 1ms) {
			data.accumulatedError -= 1ms;
			return std::ceil(data.preciseInterval.count());
		}
		else {
			return std::floor(data.preciseInterval.count());
		}
	}
}

std::uint32_t tr::EventQueue::drawTickerCallback(std::uint32_t interval, void* ptr) noexcept
{
	auto& data{*(TickerData*)(ptr)};
	data.queue.pushEvent(CustomEventBase{.type = EventType::DRAW});
	data.accumulatedError += MillisecondsD{std::fmod(data.preciseInterval.count(), 1.0)};
	if (data.accumulatedError >= 1ms) {
		data.accumulatedError -= 1ms;
		return std::ceil(data.preciseInterval.count());
	}
	else {
		return data.preciseInterval.count();
	}
}

void tr::EventQueue::sendDrawEvents(unsigned int frequency)
{
	if (_drawTicker != NO_DRAW_EVENTS) {
		if (frequency == NO_DRAW_EVENTS) {
			SDL_RemoveTimer(_drawTicker);
			_tickerData.erase(_drawTicker);
			_drawTicker = NO_DRAW_EVENTS;
		}
		else {
			_tickerData[_drawTicker]->preciseInterval  = 1000.0ms / frequency;
			_tickerData[_drawTicker]->accumulatedError = 0.0ms;
		}
	}
	else {
		auto tickerData{std::make_unique<TickerData>(std::ref(*this), 0, 1000.0ms / frequency, 0.0ms, 0)};
		auto ticker{SDL_AddTimer(1'000 / frequency, drawTickerCallback, tickerData.get())};
		if (ticker == 0) {
			throw SDLError{"Failed to add draw event ticker to event queue"};
		}
		_tickerData.emplace(ticker, std::move(tickerData));
		_drawTicker = ticker;
	}
}

void tr::EventQueue::sendTextInputEvents(bool arg) noexcept
{
	arg ? SDL_StartTextInput() : SDL_StopTextInput();
}

void tr::EventQueue::pushEvent(const Event& event)
{
	if (SDL_PushEvent((SDL_Event*)(&event)) < 0) {
		throw SDLError{"Failed to push event to event queue"};
	}
}
