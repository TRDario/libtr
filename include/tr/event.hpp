#pragma once
#define BOOST_STATIC_STRING_STANDALONE
#include "keyboard.hpp"
#include "mouse.hpp"
#include "sdl.hpp"
#include "timer.hpp"
#include <any>

namespace tr {
	class Event;
	class EventQueue;

	/** @ingroup system
	 *  @defgroup event Events
	 *  Event handling functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown when an event operation fails.
	 ******************************************************************************************************************/
	struct EventPushError : SDLError {
		using SDLError::SDLError;
	};

	/******************************************************************************************************************
	 * Namespace containing event type IDs.
	 ******************************************************************************************************************/
	namespace event_type {
		/**************************************************************************************************************
		 * ID for KeyDownEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t KEY_DOWN{0x300};

		/**************************************************************************************************************
		 * ID for KeyUpEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t KEY_UP{0x301};

		/**************************************************************************************************************
		 * ID for TextEditEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t TEXT_EDIT{0x302};

		/**************************************************************************************************************
		 * ID for TextInputEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t TEXT_INPUT{0x303};

		/**************************************************************************************************************
		 * ID for a keymap change event (no associated type).
		 **************************************************************************************************************/
		inline constexpr std::uint32_t KEYMAP_CHANGE{0x304};

		/**************************************************************************************************************
		 * ID for a clipboard update event (no associated type).
		 **************************************************************************************************************/
		inline constexpr std::uint32_t CLIPBOARD_UPDATE{0x900};

		/**************************************************************************************************************
		 * ID for MouseMotionEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t MOUSE_MOTION{0x400};

		/**************************************************************************************************************
		 * ID for MouseDownEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t MOUSE_DOWN{0x401};

		/**************************************************************************************************************
		 * ID for MouseUpEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t MOUSE_UP{0x402};

		/**************************************************************************************************************
		 * ID for MouseWheelEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t MOUSE_WHEEL{0x403};

		/**************************************************************************************************************
		 * ID for a quit event (no associated type).
		 **************************************************************************************************************/
		inline constexpr std::uint32_t QUIT{0x100};

		/**************************************************************************************************************
		 * ID for WindowEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t WINDOW{0x200};

		/**************************************************************************************************************
		 * ID for TickEvent.
		 **************************************************************************************************************/
		inline constexpr std::uint32_t TICK{0x8000};

		/**************************************************************************************************************
		 * ID for a draw event (no associated type).
		 **************************************************************************************************************/
		inline constexpr std::uint32_t DRAW{0x8001};
	} // namespace event_type

	/******************************************************************************************************************
	 * ID of the first user defined event.
	 ******************************************************************************************************************/
	inline constexpr std::uint32_t USER_EVENT_START{0x9000};

	/******************************************************************************************************************
	 * Intermediate interface between custom event types and Event.
	 ******************************************************************************************************************/
	struct CustomEventBase {
		/**************************************************************************************************************
		 * The type of the event.
		 **************************************************************************************************************/
		std::uint32_t type;

		/**************************************************************************************************************
		 * An unsigned integer value.
		 **************************************************************************************************************/
		std::uint32_t uint;

		/**************************************************************************************************************
		 * A signed integer value.
		 **************************************************************************************************************/
		std::int32_t sint;

		/**************************************************************************************************************
		 * A generic value.
		 **************************************************************************************************************/
		std::any any1;

		/**************************************************************************************************************
		 * A generic value.
		 **************************************************************************************************************/
		std::any any2;

		/**************************************************************************************************************
		 * Constructs a simple event base.
		 *
		 * @param type The type of the event.
		 **************************************************************************************************************/
		CustomEventBase(std::uint32_t type) noexcept;

		/**************************************************************************************************************
		 * Constructs a simple event base.
		 *
		 * @param type The type of the event.
		 * @param uint An unsigned integer value.
		 * @param sint A signed integer value.
		 **************************************************************************************************************/
		CustomEventBase(std::uint32_t type, std::uint32_t uint, std::int32_t sint) noexcept;

		/**************************************************************************************************************
		 * Constructs an event base.
		 *
		 * @param type The type of the event.
		 * @param uint An unsigned integer value.
		 * @param sint A signed integer value.
		 * @param any1 An any value that will be moved into the event.
		 * @param any2 An any value that will be moved into the event.
		 **************************************************************************************************************/
		CustomEventBase(std::uint32_t type, std::uint32_t uint, std::int32_t sint, std::any&& any1,
						std::any&& any2) noexcept;

		/**************************************************************************************************************
		 * Constructs an event base.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception std::bad_alloc If an any allocation fails.
		 *
		 * @param type The type of the event.
		 * @param uint An unsigned integer value.
		 * @param sint A signed integer value.
		 * @param any1 A value that will be forwarded into an instance of std::any that will be moved into the event.
		 * @param any2 A value that will be forwarded into an instance of std::any that will be moved into the event.
		 **************************************************************************************************************/
		template <class T1, class T2>
		CustomEventBase(std::uint32_t type, std::uint32_t uint, std::int32_t sint, T1&& any1, T2&& any2)
			: CustomEventBase{type, uint, sint, std::any{std::forward<T1>(any1)}, std::any{std::forward<T2>(any2)}}
		{
		}

		/**************************************************************************************************************
		 * Converts an event into an event base.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception std::bad_alloc If copying dynamically allocated resources fails.
		 *
		 * @param event
		 * @parblock
		 * The event to convert into an event base. If it holds any dynamically allocated resources, those will be
		 * copied into the custom event base.
		 *
		 * @pre The event must have a user event type.
		 * @endparblock
		 **************************************************************************************************************/
		CustomEventBase(const Event& event);

		/**************************************************************************************************************
		 * Converts an event into an event base.
		 *
		 * @param event
		 * @parblock
		 * The event to convert into an event base. If it holds any dynamically allocated resources, those will be
		 * moved into the custom event base.
		 *
		 * @pre The event must have a user event type.
		 * @endparblock
		 **************************************************************************************************************/
		CustomEventBase(Event&& event) noexcept;

		/**************************************************************************************************************
		 * Converts an event base into an event.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception std::bad_alloc If copying dynamically allocated resources fails.
		 *
		 * @return A generic event. If the event base holds any dynamically allocated resources, those will be copied
		 *         into the event.
		 **************************************************************************************************************/
		operator Event() const&;

		/**************************************************************************************************************
		 * Converts an event base into an event.
		 *
		 * @return A generic event. If the event base holds any dynamically allocated resources, those will be moved
		 *         into the event.
		 **************************************************************************************************************/
		operator Event() && noexcept;
	};

	/******************************************************************************************************************
	 * Unified event type.
	 *
	 * Event is copyable and movable.
	 *
	 * @note To handle individual event types, the type() method must be called and the event cast into the proper type.
	 ******************************************************************************************************************/
	class Event {
	  public:
		/**************************************************************************************************************
		 * Copy-constructs an event.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception std::bad_alloc If copying dynamically allocated resources fails.
		 *
		 * @param r The event to copy from.
		 **************************************************************************************************************/
		Event(const Event& r);

		/**************************************************************************************************************
		 * Move-constructs an event.
		 *
		 * @param r The event to move from. Any dynamically allocated resources will be moved into the left-hand
		 *          event, and @em r will be left in a state where another event can be moved into it, but is
		 *          otherwise unusable.
		 **************************************************************************************************************/
		Event(Event&& r) noexcept;

		/**************************************************************************************************************
		 * Destroys the event, deallocating any dynamically allocated resources it owned.
		 **************************************************************************************************************/
		~Event() noexcept;

		/**************************************************************************************************************
		 * Copy-assigns an event.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception std::bad_alloc If copying dynamically allocated resources fails.
		 *
		 * @param r The event to copy from.
		 *
		 * @return A reference to the left-hand event.
		 **************************************************************************************************************/
		Event& operator=(const Event& r);

		/**************************************************************************************************************
		 * Move-assigns an event.
		 *
		 * @param r The event to move from. Any dynamically allocated resources will be moved into the left-hand event,
		 *			and @em r will be left in a state where another event can be moved into it, but is otherwise
		 *          unusable.
		 *
		 * @return A reference to the left-hand event.
		 **************************************************************************************************************/
		Event& operator=(Event&& r) noexcept;

		/**************************************************************************************************************
		 * Gets the type ID of the event.
		 *
		 * @return The type ID of the event.
		 **************************************************************************************************************/
		std::uint32_t type() const noexcept;

	  private:
		alignas(8) std::byte _impl[56];

		Event() noexcept = default;

		friend struct CustomEventBase;
		friend class EventQueue;

		friend struct KeyDownEvent;
		friend struct KeyUpEvent;
		friend struct TextEditEvent;
		friend struct TextInputEvent;
		friend struct MouseMotionEvent;
		friend struct MouseDownEvent;
		friend struct MouseUpEvent;
		friend struct MouseWheelEvent;
		friend struct WindowEvent;
	};

	/******************************************************************************************************************
	 * Event emitted when a key is pressed.
	 *
	 * KeyDownEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct KeyDownEvent {
		/**************************************************************************************************************
		 * Whether the event is a repeat.
		 **************************************************************************************************************/
		bool repeat;

		/**************************************************************************************************************
		 * Information about the pressed key.
		 **************************************************************************************************************/
		KeyInfo key;

		/**************************************************************************************************************
		 * Constructs a key down event.
		 *
		 * @param repeat Whether the event is a repeat.
		 * @param key Information about the pressed key.
		 **************************************************************************************************************/
		KeyDownEvent(bool repeat, KeyInfo key) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a key down event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::KEY_DOWN.
		 * @endparblock
		 **************************************************************************************************************/
		KeyDownEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Event emitted when a key is release.
	 *
	 * KeyUpEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct KeyUpEvent {
		/**************************************************************************************************************
		 * Information about the released key.
		 **************************************************************************************************************/
		KeyInfo key;

		/**************************************************************************************************************
		 * Constructs a key up event.
		 *
		 * @param key Information about the released key.
		 **************************************************************************************************************/
		KeyUpEvent(KeyInfo key) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a key up event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::KEY_UP.
		 * @endparblock
		 **************************************************************************************************************/
		KeyUpEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Event emitted when text is edited.
	 *
	 * TextEditEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct TextEditEvent {
		/**************************************************************************************************************
		 * Character buffer holding the edited text string.
		 **************************************************************************************************************/
		std::array<char, 32> text{};

		/**************************************************************************************************************
		 * The selected substring.
		 **************************************************************************************************************/
		std::string_view selected;

		/**************************************************************************************************************
		 * Constructs a text editing event.
		 *
		 * @param text The edited text string.
		 * @param selected The selected substring.
		 **************************************************************************************************************/
		TextEditEvent(const std::array<char, 32>& text, std::string_view selected) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a text editing event.
		 *
		 * @param event
		 * @parblock
		 * The base event to co{}nvert.
		 *
		 * @pre The type of the event must be event_type::TEXT_EDIT.
		 * @endparblock
		 **************************************************************************************************************/
		TextEditEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Event emitted when text is inputted.
	 *
	 * TextInputEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct TextInputEvent {
		/**************************************************************************************************************
		 * Character buffer holding the inputted text string.
		 **************************************************************************************************************/
		std::array<char, 32> text{};

		/**************************************************************************************************************
		 * Constructs a text input event.
		 *
		 * @param text The inputted text string.
		 **************************************************************************************************************/
		TextInputEvent(const std::array<char, 32>& text) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a text input event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::TEXT_INPUT.
		 * @endparblock
		 **************************************************************************************************************/
		TextInputEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Event emitted when the mouse is moved.
	 *
	 * MouseMotionEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct MouseMotionEvent {
		/**************************************************************************************************************
		 * A mask of the held mouse buttons.
		 **************************************************************************************************************/
		MouseButtonMask buttons;

		/**************************************************************************************************************
		 * The position of the mouse.
		 **************************************************************************************************************/
		glm::ivec2 pos;

		/**************************************************************************************************************
		 * The change in mouse position since the last event of this type.
		 **************************************************************************************************************/
		glm::ivec2 delta;

		/**************************************************************************************************************
		 * Constructs a mouse motion event.
		 *
		 * @param buttons A mask of the held mouse buttons.
		 * @param pos The position of the mouse.
		 * @param delta The change in mouse position since the last event of this type.
		 **************************************************************************************************************/
		MouseMotionEvent(MouseButtonMask buttons, glm::ivec2 pos, glm::ivec2 delta) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a mouse motion event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::MOUSE_MOTION.
		 * @endparblock
		 **************************************************************************************************************/
		MouseMotionEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Event emitted when a mouse button is pressed.
	 *
	 * MouseDownEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct MouseDownEvent {
		/**************************************************************************************************************
		 * The pressed mouse button.
		 **************************************************************************************************************/
		MouseButton button;

		/**************************************************************************************************************
		 * The number of consecutive clicks.
		 **************************************************************************************************************/
		std::uint8_t clicks;

		/**************************************************************************************************************
		 * The position of the mouse.
		 **************************************************************************************************************/
		glm::ivec2 pos;

		/**************************************************************************************************************
		 * Constructs a mouse down event.
		 *
		 * @param button The pressed mouse button.
		 * @param clicks The number of consecutive clicks.
		 * @param pos The position of the mouse.
		 **************************************************************************************************************/
		MouseDownEvent(MouseButton button, std::uint8_t clicks, glm::ivec2 pos) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a mouse down event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::MOUSE_DOWN.
		 * @endparblock
		 **************************************************************************************************************/
		MouseDownEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Event emitted when a mouse button is released.
	 *
	 * MouseUpEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct MouseUpEvent {
		/**************************************************************************************************************
		 * The released mouse button.
		 **************************************************************************************************************/
		MouseButton button;

		/**************************************************************************************************************
		 * The position of the mouse.
		 **************************************************************************************************************/
		glm::ivec2 pos;

		/**************************************************************************************************************
		 * Constructs a mouse up event.
		 *
		 * @param button The released mouse button.
		 * @param pos The position of the mouse.
		 **************************************************************************************************************/
		MouseUpEvent(MouseButton button, glm::ivec2 pos) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a mouse wheel event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::MOUSE_UP.
		 * @endparblock
		 **************************************************************************************************************/
		MouseUpEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Event emitted when the mouse wheel is moved.
	 *
	 * MouseWheelEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct MouseWheelEvent {
		/**************************************************************************************************************
		 * The change in wheel value.
		 **************************************************************************************************************/
		glm::vec2 delta;

		/**************************************************************************************************************
		 * The position of the mouse.
		 **************************************************************************************************************/
		glm::ivec2 mousePos;

		/**************************************************************************************************************
		 * Constructs a mouse wheel event.
		 *
		 * @param delta The change in wheel value.
		 * @param mousePos The position of the mouse.
		 **************************************************************************************************************/
		MouseWheelEvent(glm::vec2 delta, glm::ivec2 mousePos) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a mouse up event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::MOUSE_WHEEL.
		 * @endparblock
		 **************************************************************************************************************/
		MouseWheelEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Event emitted when the mouse enters a window.
	 ******************************************************************************************************************/
	struct WindowEnterEvent {};

	/******************************************************************************************************************
	 * Event emitted when the mouse leaves a window.
	 ******************************************************************************************************************/
	struct WindowLeaveEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window is shown.
	 ******************************************************************************************************************/
	struct WindowShowEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window is hidden.
	 ******************************************************************************************************************/
	struct WindowHideEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window is exposed.
	 ******************************************************************************************************************/
	struct WindowExposeEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window is moved.
	 ******************************************************************************************************************/
	struct WindowMotionEvent {
		/**************************************************************************************************************
		 * The new position of the top-left corner of the window.
		 **************************************************************************************************************/
		glm::ivec2 pos;
	};

	/******************************************************************************************************************
	 * Event emitted when a window is resized.
	 ******************************************************************************************************************/
	struct WindowResizeEvent {
		/**************************************************************************************************************
		 * The new size of the window.
		 **************************************************************************************************************/
		glm::ivec2 size;
	};

	/******************************************************************************************************************
	 * Event emitted when a window's size changes.
	 ******************************************************************************************************************/
	struct WindowSizeChangeEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window is minimized.
	 ******************************************************************************************************************/
	struct WindowMinimizeEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window is maximized.
	 ******************************************************************************************************************/
	struct WindowMaximizeEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window is restored.
	 ******************************************************************************************************************/
	struct WindowRestoreEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window gains focus.
	 ******************************************************************************************************************/
	struct WindowGainFocusEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window loses focus.
	 ******************************************************************************************************************/
	struct WindowLoseFocusEvent {};

	/******************************************************************************************************************
	 * Event emitted when a window close request is made.
	 ******************************************************************************************************************/
	struct WindowCloseEvent {};

	/******************************************************************************************************************
	 * Union of window event types.
	 ******************************************************************************************************************/
	struct WindowEvent
		: public std::variant<WindowEnterEvent, WindowLeaveEvent, WindowShowEvent, WindowHideEvent, WindowExposeEvent,
							  WindowMotionEvent, WindowResizeEvent, WindowSizeChangeEvent, WindowMinimizeEvent,
							  WindowMaximizeEvent, WindowRestoreEvent, WindowGainFocusEvent, WindowLoseFocusEvent,
							  WindowCloseEvent> {
		using std::variant<WindowEnterEvent, WindowLeaveEvent, WindowShowEvent, WindowHideEvent, WindowExposeEvent,
						   WindowMotionEvent, WindowResizeEvent, WindowSizeChangeEvent, WindowMinimizeEvent,
						   WindowMaximizeEvent, WindowRestoreEvent, WindowGainFocusEvent, WindowLoseFocusEvent,
						   WindowCloseEvent>::variant;

		/**************************************************************************************************************
		 * Converts a generic event into a window event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::WINDOW.
		 * @endparblock
		 **************************************************************************************************************/
		WindowEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * The event type emitted by ticker timers.
	 *
	 * TickEvent is trivially copyable/movable.
	 ******************************************************************************************************************/
	struct TickEvent {
		/**************************************************************************************************************
		 * The user-assigned type ID attached to the ticker emitting the event.
		 **************************************************************************************************************/
		std::uint32_t id;

		/**************************************************************************************************************
		 * Constructs a tick event.
		 *
		 * @param id The tick type ID.
		 **************************************************************************************************************/
		TickEvent(std::uint32_t id) noexcept;

		/**************************************************************************************************************
		 * Converts a generic event into a tick event.
		 *
		 * @param event
		 * @parblock
		 * The base event to convert.
		 *
		 * @pre The type of the event must be event_type::TICK.
		 * @endparblock
		 **************************************************************************************************************/
		TickEvent(const Event& event) noexcept;

		/**************************************************************************************************************
		 * Converts the event into a generic event.
		 *
		 * @return A generic event.
		 **************************************************************************************************************/
		operator Event() const noexcept;
	};

	/******************************************************************************************************************
	 * Creates a timer that sends tick events.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception std::system_error If launching a thread for the timer failed.
	 * @exception std::bad_alloc If allocating the callback function failed.
	 *
	 * @param frequency The ticking frequency.
	 * @param id The ID of the tick events emitted by the timer.
	 *
	 * @return A timer that sends tick events.
	 ******************************************************************************************************************/
	Timer createTickerTimer(unsigned int frequency, std::uint32_t id);

	/******************************************************************************************************************
	 * Creates a timer that sends draw events.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception std::system_error If launching a thread for the timer failed.
	 * @exception std::bad_alloc If allocating the callback function failed.
	 *
	 * @param frequency The drawing frequency.
	 *
	 * @return A timer that sends draw events.
	 ******************************************************************************************************************/
	Timer createDrawTimer(unsigned int frequency);

	/******************************************************************************************************************
	 * Creates a timer that sends draw events with an automatically deduced frequency (fullscreen refresh rate or
	 * desktop mode refresh rate).
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception std::system_error If launching a thread for the timer failed.
	 * @exception std::bad_alloc If allocating the callback function failed.
	 *
	 * @return A timer that sends draw events.
	 ******************************************************************************************************************/
	Timer createDrawTimer();

	/******************************************************************************************************************
	 * Global event queue.
	 *
	 * This type cannot be directly instantiated.
	 ******************************************************************************************************************/
	class EventQueue {
	  public:
		/**************************************************************************************************************
		 * Polls for an event, returning it from the event queue if it exists.
		 *
		 * @return The polled event, if one was found.
		 **************************************************************************************************************/
		std::optional<Event> poll() noexcept;

		/**************************************************************************************************************
		 * Gets an event from the event queue, or waits until one appears.
		 *
		 * @return The polled event.
		 **************************************************************************************************************/
		Event wait() noexcept;

		/**************************************************************************************************************
		 * Gets an event from the event queue, waiting until one appears or until a certain amount of time has passed.
		 *
		 * @param[in] timeout The maximum amount of time to wait before returning.
		 *
		 * @return The polled event, if one was found.
		 **************************************************************************************************************/
		std::optional<Event> wait(MillisecondsI timeout) noexcept;

		/**************************************************************************************************************
		 * Handles all available events in a loop.
		 *
		 * @tparam Fn A type invocable with an Event.
		 *
		 * @param[in] fn The event handler to call.
		 **************************************************************************************************************/
		template <std::invocable<Event> Fn>
		void handle(const Fn& fn) noexcept(noexcept(std::declval<Fn>()(std::declval<Event>())));

		/**************************************************************************************************************
		 * Sets whether text input events should be sent to the event queue.
		 *
		 * @param[in] arg Whether text input events should be sent.
		 **************************************************************************************************************/
		void sendTextInputEvents(bool arg) noexcept;

		/**************************************************************************************************************
		 * Pushes an event to the queue.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception std::bad_alloc If copying dynamically allocated resources fails.
		 * @exception EventPushError If pushing the event failed.
		 *
		 * @param[in] event The event to push. Any dynamically allocated resources will be copied.
		 **************************************************************************************************************/
		void push(const Event& event);

		/**************************************************************************************************************
		 * Pushes an event to the queue.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception EventPushError If pushing the event failed.
		 *
		 * @param[in] event The event to push. Any dynamically allocated resources will be moved into the pushed event.
		 **************************************************************************************************************/
		void push(Event&& event);

	  private:
		EventQueue() noexcept = default;

		friend class Window;
	};

	/// @}
} // namespace tr

/// @cond

template <std::invocable<tr::Event> Fn>
void tr::EventQueue::handle(const Fn& fn) noexcept(noexcept(std::declval<Fn>()(std::declval<Event>())))
{
	for (std::optional<Event> event = wait(MillisecondsI{1}); event.has_value(); event = poll()) {
		fn(*event);
	}
}

/// @endcond
