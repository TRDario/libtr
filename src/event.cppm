module;
#include <cassert>
#include <SDL2/SDL.h>

export module tr:event;

import std;
import glm;
import boost;
import :chrono;
import :keyboard;
import :mouse;
import :sdl;
import :window;

using namespace std::chrono_literals;

export namespace tr {
	// Generates a new valid event type id.
	std::uint32_t generateEventType() noexcept;

	namespace EventType {
		inline constexpr std::uint32_t KEY_DOWN   	    { 0x300 };
		inline constexpr std::uint32_t KEY_UP     	    { 0x301 };
		inline constexpr std::uint32_t TEXT_EDIT  	    { 0x302 };
		inline constexpr std::uint32_t TEXT_INPUT       { 0x303 };
		inline constexpr std::uint32_t KEYMAP_CHANGE	{ 0x304 };
		inline constexpr std::uint32_t CLIPBOARD_UPDATE { 0x900 };
		inline constexpr std::uint32_t MOUSE_MOTION	    { 0x400 };
		inline constexpr std::uint32_t MOUSE_DOWN	    { 0x401 };
		inline constexpr std::uint32_t MOUSE_UP		    { 0x402 };
		inline constexpr std::uint32_t MOUSE_WHEEL	    { 0x403 };
		inline constexpr std::uint32_t QUIT 			{ 0x100 };
		inline constexpr std::uint32_t WINDOW		    { 0x200 };
		inline const 	 std::uint32_t TICK 			{ generateEventType() };
		inline const 	 std::uint32_t DRAW 			{ generateEventType() };
	}
	
	struct KeyDownEvent          { WindowView win; bool repeat; KeyInfo key; };
	struct KeyUpEvent            { WindowView win; KeyInfo key; };
	struct TextEditEvent         { WindowView win; boost::static_string<31> text; int selbeg; int sellen; };
	struct TextInputEvent        { WindowView win; boost::static_string<31> text; };
	struct MouseMotionEvent 	 { WindowView win; MouseButtonMask buttons; glm::ivec2 pos; glm::ivec2 delta; };
	struct MouseDownEvent 		 { WindowView win; MouseButton button; std::uint8_t clicks; glm::ivec2 pos; };
	struct MouseUpEvent			 { WindowView win; MouseButton button; glm::ivec2 pos; };
	struct MouseWheelEvent		 { WindowView win; glm::vec2 delta; glm::ivec2 mousePos; };
	struct WindowEnterEvent		 { WindowView win; };
	struct WindowLeaveEvent		 { WindowView win; };
	struct WindowShowEvent		 { WindowView win; };
	struct WindowHideEvent		 { WindowView win; };
	struct WindowExposeEvent	 { WindowView win; };
	struct WindowMotionEvent 	 { WindowView win; glm::ivec2 pos; };
	struct WindowResizeEvent 	 { WindowView win; glm::ivec2 size; };
	struct WindowSizeChangeEvent { WindowView win; };
	struct WindowMinimizeEvent   { WindowView win; };
	struct WindowMaximizeEvent   { WindowView win; }; 
	struct WindowRestoreEvent    { WindowView win; };
	struct WindowGainFocusEvent  { WindowView win; };
	struct WindowLoseFocusEvent  { WindowView win; };
	struct WindowCloseEvent      { WindowView win; };
	struct HitTestEvent 		 { WindowView win; };
	using  WindowEvent = std::variant<WindowEnterEvent, WindowLeaveEvent, WindowShowEvent, WindowHideEvent, WindowExposeEvent, WindowMotionEvent, WindowResizeEvent, WindowSizeChangeEvent, WindowMinimizeEvent, WindowMaximizeEvent, WindowRestoreEvent, WindowGainFocusEvent, WindowLoseFocusEvent, WindowCloseEvent, HitTestEvent>;
	struct TickEvent 			 { std::uint32_t id; };

	// Intermediate interface between custom event types and Event.
	struct CustomEventBase {
		std::uint32_t     type;
		std::uint32_t     uint;
		std::int32_t     sint;
		std::any any1;
		std::any any2;
	};

	// Unified event type.
	class Event {
	public:
		Event(const CustomEventBase& custom);

		// Gets the type ID of the event.
		std::uint32_t type() const noexcept;

		operator KeyDownEvent() const noexcept;
		operator KeyUpEvent() const noexcept;
		operator TextEditEvent() const noexcept;
		operator TextInputEvent() const noexcept;
		operator MouseMotionEvent() const noexcept;
		operator MouseDownEvent() const noexcept;
		operator MouseUpEvent() const noexcept;
		operator MouseWheelEvent() const noexcept;
		operator WindowEvent() const noexcept;
		operator TickEvent() const noexcept;
		template <std::constructible_from<CustomEventBase> T> operator T() const noexcept;
	private:
		SDL_Event _impl;

        Event() noexcept = default;

		CustomEventBase getCustomEventBase() const noexcept;

		friend class EventQueue;
	};

	// Ticker handle.
	enum class Ticker : int {};
	// Sentinel value that signals that a ticker should tick forever.
	[[maybe_unused]] inline constexpr std::uint32_t TICK_FOREVER { 0 };
	// Sentinel value that signals that no draw events should be emitted.
	[[maybe_unused]] inline constexpr std::uint32_t NO_DRAW_EVENTS { 0 };

	// Class representing the event queue.
	class EventQueue {
	public:
		EventQueue() noexcept = default;
		EventQueue(const EventQueue&) = delete;
		EventQueue(EventQueue&&) = default;
		~EventQueue() noexcept;

		void operator=(const EventQueue&) = delete;
		EventQueue& operator=(EventQueue&&) = default;

		// Polls for an event, returning it from the event queue if it exists.
		std::optional<Event> pollEvent() noexcept;
		// Gets an event from the event queue, or waits until one appears.
		Event waitForEvent() noexcept;
		// Gets an event from the event queue, waiting until one appears or until a certain amount of time has passed.
		std::optional<Event> waitForEventTimeout(std::chrono::milliseconds timeout) noexcept;

		// Adds a ticker that generates tick events at a regular interval.
		Ticker addTicker(std::int32_t id, MillisecondsD interval, std::uint32_t nticks);
		// Halts a ticker.
		void removeTicker(Ticker ticker) noexcept;

		// Sets the frequency at which draw events are sent at or NO_DRAW_EVENTS.
		void sendDrawEvents(unsigned int frequency);
		// Sets whether text input events should be sent to the event queue.
		void sendTextInputEvents(bool arg) noexcept;

		// Pushes an event to the queue.
		void pushEvent(const Event& event);
	private:
		// Data used by a ticker.
		struct TickerData {
			EventQueue&   queue;
			std::int32_t  id;
			MillisecondsD preciseInterval;
			MillisecondsD accumulatedError; // The accumulated error caused by imprecise SDL timers, used for correction.
			std::uint32_t ticksLeft; 	    // The number of ticks left before automatically halting or TICK_FOREVER.
		};

		static std::uint32_t tickerCallback(std::uint32_t interval, void* ptr) noexcept;
		static std::uint32_t drawTickerCallback(std::uint32_t interval, void* ptr) noexcept;

		std::unordered_map<int, std::unique_ptr<TickerData>> _tickerData;
		int 							                     _drawTicker = 0; // The ID of the draw event ticker.
	};
}

// IMPLEMENTATION

template <std::constructible_from<tr::CustomEventBase> T>
tr::Event::operator T() const noexcept
{
	return T(getCustomEventBase());
}

std::uint32_t tr::generateEventType() noexcept 
{
    return SDL_RegisterEvents(1);
}

tr::Event::Event(const CustomEventBase& custom)
{
    _impl.type = custom.type;
    _impl.user.windowID = custom.uint;
    _impl.user.code = custom.sint;
    _impl.user.data1 = new std::any { std::move(custom.any1) };
    _impl.user.data2 = new std::any { std::move(custom.any2) };
}

std::uint32_t tr::Event::type() const noexcept
{
    return _impl.type;
}

tr::Event::operator KeyDownEvent() const noexcept
{
    assert(type() == EventType::KEY_DOWN);
    return {
        .win = WindowView { SDL_GetWindowFromID(_impl.key.windowID) },
        .repeat = bool(_impl.key.repeat),
        .key {
            .scan = Scancode::Enum(_impl.key.keysym.scancode),
            .key = Keycode::Enum(_impl.key.keysym.sym),
            .mods = Keymods(_impl.key.keysym.mod)
        }
    };
}

tr::Event::operator KeyUpEvent() const noexcept
{
    assert(type() == EventType::KEY_UP);
    return {
        .win = WindowView { SDL_GetWindowFromID(_impl.key.windowID) },
        .key = {
            .scan = Scancode::Enum(_impl.key.keysym.scancode),
            .key = Keycode::Enum(_impl.key.keysym.sym),
            .mods = Keymods(_impl.key.keysym.mod)
        }
    };
}

tr::Event::operator TextEditEvent() const noexcept
{
    assert(type() == EventType::TEXT_EDIT);
    return {
        .win = WindowView { SDL_GetWindowFromID(_impl.edit.windowID) },
        .text = _impl.edit.text,
        .selbeg = _impl.edit.start,
        .sellen = _impl.edit.length,
    };
}

tr::Event::operator TextInputEvent() const noexcept
{
    assert(type() == EventType::TEXT_INPUT);
    return {
        .win = WindowView { SDL_GetWindowFromID(_impl.text.windowID) },
        .text = _impl.text.text
    };
}

tr::Event::operator MouseMotionEvent() const noexcept
{
    assert(type() == EventType::MOUSE_MOTION);
    return {
        .win = WindowView { SDL_GetWindowFromID(_impl.motion.windowID) },
        .buttons = MouseButtonMask(_impl.motion.state),
        .pos = { _impl.motion.x, _impl.motion.y },
        .delta = { _impl.motion.xrel, _impl.motion.yrel }
    };
}

tr::Event::operator MouseDownEvent() const noexcept
{
    assert(type() == EventType::MOUSE_DOWN);
    return {
        .win = WindowView { SDL_GetWindowFromID(_impl.button.windowID) },
        .button = MouseButton(_impl.button.button),
        .clicks = _impl.button.clicks,
        .pos = { _impl.button.x, _impl.button.y }
    };
}

tr::Event::operator MouseUpEvent() const noexcept
{
    assert(type() == EventType::MOUSE_UP);
    return {
        .win = WindowView { SDL_GetWindowFromID(_impl.button.windowID) },
        .button = MouseButton(_impl.button.button),
        .pos = { _impl.button.x, _impl.button.y }
    };
}

tr::Event::operator MouseWheelEvent() const noexcept
{
    assert(type() == EventType::MOUSE_WHEEL);
    return {
        .win = WindowView { SDL_GetWindowFromID(_impl.wheel.windowID) },
        .delta = { _impl.wheel.preciseX, _impl.wheel.preciseY },
        .mousePos { _impl.wheel.mouseX, _impl.wheel.mouseY }
    };
}

tr::Event::operator WindowEvent() const noexcept
{
    assert(type() == EventType::WINDOW);
    switch (_impl.window.event) {
    case SDL_WINDOWEVENT_ENTER:
        return WindowEnterEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_LEAVE:
        return WindowLeaveEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_SHOWN:
        return WindowShowEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_HIDDEN:
        return WindowHideEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_EXPOSED:
        return WindowExposeEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_MOVED:
        return WindowMotionEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) },
            .pos = { _impl.window.data1, _impl.window.data2 }
        };
    case SDL_WINDOWEVENT_RESIZED:
        return WindowResizeEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) },
            .size = { _impl.window.data1, _impl.window.data2 }
        };
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        return WindowSizeChangeEvent {	
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_MINIMIZED:
        return WindowMinimizeEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_MAXIMIZED:
        return WindowMaximizeEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_RESTORED:
        return WindowRestoreEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        return WindowGainFocusEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_FOCUS_LOST:
        return WindowLoseFocusEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_TAKE_FOCUS:
        SDL_SetWindowInputFocus(SDL_GetWindowFromID(_impl.window.windowID));
        break;
    case SDL_WINDOWEVENT_CLOSE:
        return WindowCloseEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    case SDL_WINDOWEVENT_HIT_TEST:
        return HitTestEvent {
            .win = WindowView { SDL_GetWindowFromID(_impl.window.windowID) }
        };
    }
    std::unreachable();
}

tr::Event::operator TickEvent() const noexcept
{
    assert(type() == EventType::TICK);
    return { .id = _impl.user.windowID };
}

tr::CustomEventBase tr::Event::getCustomEventBase() const noexcept
{
    std::unique_ptr<std::any> any1 { (std::any*)(_impl.user.data1) };
    std::unique_ptr<std::any> any2 { (std::any*)(_impl.user.data2) };
    return {
        .type = _impl.type,
        .uint = _impl.user.windowID,
        .sint = _impl.user.code,
        .any1 = std::move(*any1),
        .any2 = std::move(*any2)
    };
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
    return SDL_PollEvent(&event._impl) ? std::optional<Event> { event } : std::nullopt;
}

tr::Event tr::EventQueue::waitForEvent() noexcept
{
    Event event;
    SDL_WaitEvent(&event._impl);
    return event;
}

std::optional<tr::Event> tr::EventQueue::waitForEventTimeout(std::chrono::milliseconds timeout) noexcept
{
    Event event {};
    if (SDL_WaitEventTimeout(&event._impl, timeout.count())) {
        return event;
    }
    else {
        return std::nullopt;
    }
}

tr::Ticker tr::EventQueue::addTicker(std::int32_t id, MillisecondsD interval, std::uint32_t nticks)
{
    auto tickerData { std::make_unique<TickerData>(std::ref(*this), id, interval, 0.0ms, nticks) };
    auto ticker { SDL_AddTimer(interval.count(), tickerCallback, tickerData.get()) };
    if (ticker == 0) {
        throw SDLError { "Failed to add event ticker to event queue" };
    }
    _tickerData.emplace(ticker, std::move(tickerData));
    return Ticker { ticker };
}


void tr::EventQueue::removeTicker(Ticker ticker) noexcept
{
    SDL_RemoveTimer(SDL_TimerID(ticker));
    _tickerData.erase(std::uint32_t(ticker));
}

std::uint32_t tr::EventQueue::tickerCallback(std::uint32_t interval, void* ptr) noexcept
{
    auto& data { *(TickerData*)(ptr) };
    data.queue.pushEvent(CustomEventBase { .type = EventType::TICK, .uint = std::uint32_t(data.id) });
    if (data.ticksLeft != TICK_FOREVER && --data.ticksLeft == 0) {
        return 0;
    }
    else {
        data.accumulatedError += data.preciseInterval - std::chrono::milliseconds { interval };
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
    auto& data { *(TickerData*)(ptr) };
    data.queue.pushEvent(CustomEventBase { .type = EventType::DRAW });
    data.accumulatedError += MillisecondsD { std::fmod(data.preciseInterval.count(), 1.0) };
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
            _tickerData[_drawTicker]->preciseInterval = 1000.0ms / frequency;
            _tickerData[_drawTicker]->accumulatedError = 0.0ms;
        }
    }
    else {
        auto tickerData { std::make_unique<TickerData>(std::ref(*this), 0, 1000.0ms / frequency, 0.0ms, 0) };
        auto ticker { SDL_AddTimer(1000 / frequency, drawTickerCallback, tickerData.get()) };
        if (ticker == 0) {
            throw SDLError { "Failed to add draw event ticker to event queue" };
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
    if (SDL_PushEvent(const_cast<SDL_Event*>(&event._impl)) < 0) {
        throw SDLError { "Failed to push event to event queue" };
    }
}