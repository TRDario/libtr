#include "../include/tr/overloaded_lambda.hpp"
#include "../include/tr/ranges.hpp"
#include "../include/tr/timer.hpp"
#include "../include/tr/window.hpp"
#include <SDL2/SDL.h>

using namespace std::chrono_literals;

tr::CustomEventBase::CustomEventBase(std::uint32_t type) noexcept
	: type{type}
{
}

tr::CustomEventBase::CustomEventBase(std::uint32_t type, std::uint32_t uint, std::int32_t sint) noexcept
	: type{type}, uint{uint}, sint{sint}
{
}

tr::CustomEventBase::CustomEventBase(std::uint32_t type, std::uint32_t uint, std::int32_t sint, std::any&& any1,
									 std::any&& any2) noexcept
	: type{type}, uint{uint}, sint{sint}, any1{std::move(any1)}, any2{std::move(any2)}
{
}

tr::CustomEventBase::CustomEventBase(const Event& event)
{
	assert(event.type() >= SDL_USEREVENT);

	const SDL_UserEvent& sdl{reinterpret_cast<const SDL_Event*>(event._impl)->user};
	type = sdl.type;
	uint = sdl.windowID;
	sint = sdl.code;
	if (sdl.data1 != nullptr) {
		any1 = *static_cast<const std::any*>(sdl.data1);
	}
	if (sdl.data2 != nullptr) {
		any2 = *static_cast<const std::any*>(sdl.data2);
	}
}

tr::CustomEventBase::CustomEventBase(Event&& event) noexcept
{
	assert(event.type() >= SDL_USEREVENT);

	SDL_UserEvent& sdl{reinterpret_cast<SDL_Event*>(event._impl)->user};
	type = sdl.type;
	uint = sdl.windowID;
	sint = sdl.code;
	if (sdl.data1 != nullptr) {
		any1 = std::move(*static_cast<std::any*>(sdl.data1));
	}
	if (sdl.data2 != nullptr) {
		any2 = std::move(*static_cast<std::any*>(sdl.data2));
	}
	sdl.type = 0;
}

tr::CustomEventBase::operator Event() const&
{
	Event      event;
	SDL_Event& sdl{*reinterpret_cast<SDL_Event*>(event._impl)};
	sdl.type          = type;
	sdl.user.windowID = uint;
	sdl.user.code     = sint;
	sdl.user.data1    = any1.has_value() ? new std::any{any1} : nullptr;
	sdl.user.data2    = any2.has_value() ? new std::any{any2} : nullptr;
	return event;
}

tr::CustomEventBase::operator Event() && noexcept
{
	Event      event;
	SDL_Event& sdl{*reinterpret_cast<SDL_Event*>(event._impl)};
	sdl.type          = type;
	sdl.user.windowID = uint;
	sdl.user.code     = sint;
	sdl.user.data1    = any1.has_value() ? new std::any{std::move(any1)} : nullptr;
	sdl.user.data2    = any2.has_value() ? new std::any{std::move(any2)} : nullptr;
	return event;
}

tr::Event::Event(const Event& r)
{
	std::ranges::copy(r._impl, _impl);
	if (type() >= SDL_USEREVENT) {
		SDL_UserEvent&       lsdl{reinterpret_cast<SDL_Event*>(_impl)->user};
		const SDL_UserEvent& rsdl{reinterpret_cast<const SDL_Event*>(r._impl)->user};
		lsdl.data1 = lsdl.data1 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data1)} : nullptr;
		lsdl.data2 = lsdl.data2 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data2)} : nullptr;
	}
}

tr::Event::Event(Event&& r) noexcept
{
	std::ranges::copy(r._impl, _impl);
	if (type() >= SDL_USEREVENT) {
		SDL_UserEvent& rsdl{reinterpret_cast<SDL_Event*>(r._impl)->user};
		rsdl.data1 = nullptr;
		rsdl.data2 = nullptr;
		rsdl.type  = 0;
	}
}

tr::Event::~Event() noexcept
{
	if (type() >= SDL_USEREVENT) {
		SDL_UserEvent& sdl{reinterpret_cast<SDL_Event*>(_impl)->user};
		delete static_cast<std::any*>(sdl.data1);
		delete static_cast<std::any*>(sdl.data2);
	}
}

tr::Event& tr::Event::operator=(const Event& r)
{
	std::ignore = std::move(*this);
	std::ranges::copy(r._impl, _impl);
	if (type() >= SDL_USEREVENT) {
		SDL_UserEvent&       lsdl{reinterpret_cast<SDL_Event*>(_impl)->user};
		const SDL_UserEvent& rsdl{reinterpret_cast<const SDL_Event*>(r._impl)->user};
		lsdl.data1 = lsdl.data1 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data1)} : nullptr;
		lsdl.data2 = lsdl.data2 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data2)} : nullptr;
	}
	return *this;
}

tr::Event& tr::Event::operator=(Event&& r) noexcept
{
	std::ignore = std::move(*this);
	std::ranges::copy(r._impl, _impl);
	if (type() >= SDL_USEREVENT) {
		SDL_UserEvent& rsdl{reinterpret_cast<SDL_Event*>(r._impl)->user};
		rsdl.data1 = nullptr;
		rsdl.data2 = nullptr;
	}
	return *this;
}

std::uint32_t tr::Event::type() const noexcept
{
	return (reinterpret_cast<const SDL_Event*>(_impl))->type;
}

tr::KeyDownEvent::KeyDownEvent(bool repeat, KeyInfo key) noexcept
	: repeat{repeat}, key{key}
{
}

tr::KeyDownEvent::KeyDownEvent(const Event& event) noexcept
{
	assert(event.type() == event_type::KEY_DOWN);

	const SDL_KeyboardEvent& sdl{(reinterpret_cast<const SDL_Event*>(event._impl))->key};
	repeat = sdl.repeat;
	key    = {.scan = static_cast<Scancode::Enum>(sdl.keysym.scancode),
			  .key  = static_cast<Keycode::Enum>(sdl.keysym.sym),
			  .mods = static_cast<Keymods>(sdl.keysym.mod)};
}

tr::KeyDownEvent::operator Event() const noexcept
{
	Event              event;
	SDL_KeyboardEvent& sdl{(reinterpret_cast<SDL_Event*>(event._impl))->key};
	sdl.type            = SDL_KEYDOWN;
	sdl.repeat          = repeat;
	sdl.keysym.scancode = static_cast<SDL_Scancode>(static_cast<Scancode::Enum>(key.scan));
	sdl.keysym.sym      = static_cast<SDL_Keycode>(static_cast<Keycode::Enum>(key.key));
	sdl.keysym.mod      = static_cast<std::uint16_t>(key.mods);
	return event;
}

tr::KeyUpEvent::KeyUpEvent(KeyInfo key) noexcept
	: key{key}
{
}

tr::KeyUpEvent::KeyUpEvent(const Event& event) noexcept
{
	assert(event.type() == event_type::KEY_UP);

	const SDL_KeyboardEvent& sdl{(reinterpret_cast<const SDL_Event*>(event._impl))->key};
	key = {.scan = static_cast<Scancode::Enum>(sdl.keysym.scancode),
		   .key  = static_cast<Keycode::Enum>(sdl.keysym.sym),
		   .mods = static_cast<Keymods>(sdl.keysym.mod)};
}

tr::KeyUpEvent::operator Event() const noexcept
{
	Event              event;
	SDL_KeyboardEvent& sdl{(reinterpret_cast<SDL_Event*>(event._impl))->key};
	sdl.type            = SDL_KEYUP;
	sdl.keysym.scancode = static_cast<SDL_Scancode>(static_cast<Scancode::Enum>(key.scan));
	sdl.keysym.sym      = static_cast<SDL_Keycode>(static_cast<Keycode::Enum>(key.key));
	sdl.keysym.mod      = static_cast<std::uint16_t>(key.mods);
	return event;
}

tr::TextEditEvent::TextEditEvent(const std::array<char, 32>& text, std::string_view selected) noexcept
	: text{text}, selected{selected}
{
}

tr::TextEditEvent::TextEditEvent(const Event& event) noexcept
{
	const SDL_TextEditingEvent& sdl{reinterpret_cast<const SDL_Event*>(event._impl)->edit};
	assert(event.type() == event_type::TEXT_EDIT);
	std::ranges::copy(std::string_view{sdl.text}, text.begin());
	selected = {text.data() + sdl.start, static_cast<std::size_t>(sdl.length)};
}

tr::TextEditEvent::operator Event() const noexcept
{
	Event                 event;
	SDL_TextEditingEvent& sdl{reinterpret_cast<SDL_Event*>(event._impl)->edit};
	sdl.type = SDL_TEXTEDITING;
	std::ranges::copy(text, sdl.text);
	sdl.text[text.size()] = '\0';
	sdl.start             = selected.begin() - text.begin();
	sdl.length            = selected.length();
	return event;
}

tr::TextInputEvent::TextInputEvent(const std::array<char, 32>& text) noexcept
	: text{text}
{
}

tr::TextInputEvent::TextInputEvent(const Event& event) noexcept
{
	const SDL_TextInputEvent& sdl{reinterpret_cast<const SDL_Event*>(event._impl)->text};
	assert(event.type() == event_type::TEXT_INPUT);
	std::ranges::copy(std::string_view{sdl.text}, text.begin());
}

tr::TextInputEvent::operator Event() const noexcept
{
	Event               event;
	SDL_TextInputEvent& sdl{reinterpret_cast<SDL_Event*>(event._impl)->text};
	sdl.type = SDL_TEXTINPUT;
	std::ranges::copy(text, sdl.text);
	sdl.text[text.size()] = '\0';
	return event;
}

tr::MouseMotionEvent::MouseMotionEvent(MouseButtonMask buttons, glm::ivec2 pos, glm::ivec2 delta) noexcept
	: buttons{buttons}, pos{pos}, delta{delta}
{
}

tr::MouseMotionEvent::MouseMotionEvent(const Event& event) noexcept
{
	const SDL_MouseMotionEvent& sdl{reinterpret_cast<const SDL_Event*>(event._impl)->motion};
	assert(event.type() == event_type::MOUSE_MOTION);
	buttons = static_cast<MouseButtonMask>(sdl.state);
	pos     = {sdl.x, sdl.y};
	delta   = {sdl.xrel, sdl.yrel};
}

tr::MouseMotionEvent::operator Event() const noexcept
{
	Event                 event;
	SDL_MouseMotionEvent& sdl{reinterpret_cast<SDL_Event*>(event._impl)->motion};
	sdl.type  = SDL_MOUSEMOTION;
	sdl.state = static_cast<std::uint32_t>(buttons);
	sdl.x     = pos.x;
	sdl.y     = pos.y;
	sdl.xrel  = delta.x;
	sdl.yrel  = delta.y;
	return event;
}

tr::MouseDownEvent::MouseDownEvent(MouseButton button, std::uint8_t clicks, glm::ivec2 pos) noexcept
	: button{button}, clicks{clicks}, pos{pos}
{
}

tr::MouseDownEvent::MouseDownEvent(const Event& event) noexcept
{
	const SDL_MouseButtonEvent& sdl{reinterpret_cast<const SDL_Event*>(event._impl)->button};
	assert(event.type() == event_type::MOUSE_DOWN);
	button = static_cast<MouseButton>(sdl.button);
	clicks = sdl.clicks;
	pos    = {sdl.x, sdl.y};
}

tr::MouseDownEvent::operator Event() const noexcept
{
	Event                 event;
	SDL_MouseButtonEvent& sdl{reinterpret_cast<SDL_Event*>(event._impl)->button};
	sdl.type   = SDL_MOUSEBUTTONDOWN;
	sdl.button = static_cast<std::uint8_t>(button);
	sdl.clicks = clicks;
	sdl.x      = pos.x;
	sdl.y      = pos.y;
	return event;
}

tr::MouseUpEvent::MouseUpEvent(MouseButton button, glm::ivec2 pos) noexcept
	: button{button}, pos{pos}
{
}

tr::MouseUpEvent::MouseUpEvent(const Event& event) noexcept
{
	const SDL_MouseButtonEvent& sdl{reinterpret_cast<const SDL_Event*>(event._impl)->button};
	assert(event.type() == event_type::MOUSE_UP);
	button = static_cast<MouseButton>(sdl.button);
	pos    = {sdl.x, sdl.y};
}

tr::MouseUpEvent::operator Event() const noexcept
{
	Event                 event;
	SDL_MouseButtonEvent& sdl{reinterpret_cast<SDL_Event*>(event._impl)->button};
	sdl.type   = SDL_MOUSEBUTTONUP;
	sdl.button = static_cast<std::uint8_t>(button);
	sdl.x      = pos.x;
	sdl.y      = pos.y;
	return event;
}

tr::MouseWheelEvent::MouseWheelEvent(glm::vec2 delta, glm::ivec2 mousePos) noexcept
	: delta{delta}, mousePos{mousePos}
{
}

tr::MouseWheelEvent::MouseWheelEvent(const Event& event) noexcept
{
	const SDL_MouseWheelEvent& sdl{reinterpret_cast<const SDL_Event*>(event._impl)->wheel};
	assert(event.type() == event_type::MOUSE_WHEEL);
	delta    = {sdl.preciseX, sdl.preciseY};
	mousePos = {sdl.mouseX, sdl.mouseY};
}

tr::MouseWheelEvent::operator Event() const noexcept
{
	Event                event;
	SDL_MouseWheelEvent& sdl{reinterpret_cast<SDL_Event*>(event._impl)->wheel};
	sdl.type     = SDL_MOUSEWHEEL;
	sdl.preciseX = delta.x;
	sdl.preciseY = delta.y;
	sdl.mouseX   = mousePos.x;
	sdl.mouseY   = mousePos.y;
	return event;
}

tr::WindowEvent::WindowEvent(const Event& event) noexcept
{
	const SDL_WindowEvent& sdl{reinterpret_cast<const SDL_Event*>(event._impl)->window};

	assert(event.type() == event_type::WINDOW);
	switch (sdl.event) {
	case SDL_WINDOWEVENT_ENTER:
		*this = WindowEnterEvent{};
		break;
	case SDL_WINDOWEVENT_LEAVE:
		*this = WindowLeaveEvent{};
		break;
	case SDL_WINDOWEVENT_SHOWN:
		*this = WindowShowEvent{};
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		*this = WindowHideEvent{};
		break;
	case SDL_WINDOWEVENT_EXPOSED:
		*this = WindowExposeEvent{};
		break;
	case SDL_WINDOWEVENT_MOVED:
		*this = WindowMotionEvent{.pos = {sdl.data1, sdl.data2}};
		break;
	case SDL_WINDOWEVENT_RESIZED:
		*this = WindowResizeEvent{.size = {sdl.data1, sdl.data2}};
		break;
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		*this = WindowSizeChangeEvent{};
		break;
	case SDL_WINDOWEVENT_MINIMIZED:
		*this = WindowMinimizeEvent{};
		break;
	case SDL_WINDOWEVENT_MAXIMIZED:
		*this = WindowMaximizeEvent{};
		break;
	case SDL_WINDOWEVENT_RESTORED:
		*this = WindowRestoreEvent{};
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		*this = WindowGainFocusEvent{};
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		*this = WindowLoseFocusEvent{};
		break;
	case SDL_WINDOWEVENT_TAKE_FOCUS:
		SDL_SetWindowInputFocus(SDL_GetWindowFromID(sdl.windowID));
		break;
	case SDL_WINDOWEVENT_CLOSE:
		*this = WindowCloseEvent{};
		break;
	}
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
	__assume(false);
#else // GCC, Clang
	__builtin_unreachable();
#endif
}

tr::WindowEvent::operator Event() const noexcept
{
	Event            event;
	SDL_WindowEvent& sdl{reinterpret_cast<SDL_Event*>(event._impl)->window};
	sdl.type = SDL_WINDOWEVENT;
	std::visit(Overloaded{[&](const WindowEnterEvent&) { sdl.event = SDL_WINDOWEVENT_ENTER; },
						  [&](const WindowLeaveEvent&) { sdl.event = SDL_WINDOWEVENT_LEAVE; },
						  [&](const WindowShowEvent&) { sdl.event = SDL_WINDOWEVENT_SHOWN; },
						  [&](const WindowHideEvent&) { sdl.event = SDL_WINDOWEVENT_HIDDEN; },
						  [&](const WindowExposeEvent&) { sdl.event = SDL_WINDOWEVENT_EXPOSED; },
						  [&](const WindowMotionEvent& e) {
							  sdl.event = SDL_WINDOWEVENT_MOVED;
							  sdl.data1 = e.pos.x;
							  sdl.data2 = e.pos.y;
						  },
						  [&](const WindowResizeEvent& e) {
							  sdl.event = SDL_WINDOWEVENT_RESIZED;
							  sdl.data1 = e.size.x;
							  sdl.data2 = e.size.y;
						  },
						  [&](const WindowSizeChangeEvent&) { sdl.event = SDL_WINDOWEVENT_SIZE_CHANGED; },
						  [&](const WindowMinimizeEvent&) { sdl.event = SDL_WINDOWEVENT_MINIMIZED; },
						  [&](const WindowMaximizeEvent&) { sdl.event = SDL_WINDOWEVENT_MAXIMIZED; },
						  [&](const WindowRestoreEvent&) { sdl.event = SDL_WINDOWEVENT_RESTORED; },
						  [&](const WindowGainFocusEvent&) { sdl.event = SDL_WINDOWEVENT_FOCUS_GAINED; },
						  [&](const WindowLoseFocusEvent&) { sdl.event = SDL_WINDOWEVENT_FOCUS_LOST; },
						  [&](const WindowCloseEvent&) { sdl.event = SDL_WINDOWEVENT_CLOSE; }},
			   *this);
	return event;
}

tr::TickEvent::TickEvent(std::uint32_t id) noexcept
	: id{id}
{
}

tr::TickEvent::TickEvent(const Event& event) noexcept
	: id{CustomEventBase{event}.uint}
{
	assert(event.type() == event_type::TICK);
}

tr::TickEvent::operator Event() const noexcept
{
	return CustomEventBase{event_type::TICK, id, 0};
}

tr::Timer tr::createTickerTimer(unsigned int frequency, std::uint32_t id)
{
	return Timer{1.0s / frequency, [=] { window().events().push(TickEvent{id}); }};
}

tr::Timer tr::createDrawTimer(unsigned int frequency)
{
	return Timer{1.0s / frequency, [] { window().events().push(CustomEventBase{event_type::DRAW}); }};
}

std::optional<tr::Event> tr::EventQueue::poll() noexcept
{
	Event event{};
	return SDL_PollEvent(reinterpret_cast<SDL_Event*>(&event)) ? std::optional<Event>{std::move(event)} : std::nullopt;
}

tr::Event tr::EventQueue::wait() noexcept
{
	Event event;
	SDL_WaitEvent(reinterpret_cast<SDL_Event*>(&event));
	return event;
}

std::optional<tr::Event> tr::EventQueue::wait(MillisecondsI timeout) noexcept
{
	Event event{};
	if (SDL_WaitEventTimeout(reinterpret_cast<SDL_Event*>(&event), timeout.count())) {
		return event;
	}
	else {
		return std::nullopt;
	}
}

void tr::EventQueue::sendTextInputEvents(bool arg) noexcept
{
	arg ? SDL_StartTextInput() : SDL_StopTextInput();
}

void tr::EventQueue::push(const Event& event)
{
	SDL_Event sdl;
	std::ranges::copy(event._impl, asMutBytes(sdl).begin());
	if (event.type() >= SDL_USEREVENT) {
		const SDL_UserEvent& rsdl{reinterpret_cast<const SDL_Event*>(event._impl)->user};
		sdl.user.data1 = rsdl.data1 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data1)} : nullptr;
		sdl.user.data2 = rsdl.data2 != nullptr ? new std::any{*static_cast<const std::any*>(rsdl.data2)} : nullptr;
	}

	if (SDL_PushEvent(&sdl) < 0) {
		throw EventPushError{"Failed to push event to event queue"};
	}
}

void tr::EventQueue::push(Event&& event)
{
	SDL_Event sdl;
	std::ranges::copy(event._impl, asMutBytes(sdl).begin());
	if (SDL_PushEvent(&sdl) < 0) {
		throw EventPushError{"Failed to push event to event queue"};
	}

	if (event.type() >= SDL_USEREVENT) {
		SDL_UserEvent& rsdl{reinterpret_cast<SDL_Event*>(event._impl)->user};
		rsdl.data1 = nullptr;
		rsdl.data2 = nullptr;
	}
}