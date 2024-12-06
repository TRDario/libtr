#pragma once
#define BOOST_STATIC_STRING_STANDALONE
#include "chrono.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "sdl.hpp"
#include <any>
#include <atomic>
#include <boost/static_string.hpp>

namespace tr {
	class EventQueue;

	/** @defgroup event Events
	 *  Event handling functionality.
	 *
	 *  An instance of Window must be created before any other functionality from this section can be used.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown when an event operation fails.
	 ******************************************************************************************************************/
	struct EventError : SDLError {
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
	 * Event emitted when a key is pressed.
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
	};

	/******************************************************************************************************************
	 * Event emitted when a key is released.
	 ******************************************************************************************************************/
	struct KeyUpEvent {
		/**************************************************************************************************************
		 * Information about the released key.
		 **************************************************************************************************************/
		KeyInfo key;
	};

	/******************************************************************************************************************
	 * Event emitted when text is edited.
	 ******************************************************************************************************************/
	struct TextEditEvent {
		/**************************************************************************************************************
		 * The edited text string.
		 **************************************************************************************************************/
		boost::static_string<31> text;

		/**************************************************************************************************************
		 * The selected substring.
		 **************************************************************************************************************/
		std::string_view selected;
	};

	/******************************************************************************************************************
	 * Event emitted when text is inputted.
	 ******************************************************************************************************************/
	struct TextInputEvent {
		/**************************************************************************************************************
		 * The inputted text string.
		 **************************************************************************************************************/
		boost::static_string<31> text;
	};

	/******************************************************************************************************************
	 * Event emitted when the mouse is moved.
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
	};

	/******************************************************************************************************************
	 * Event emitted when a mouse button is pressed.
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
	};

	/******************************************************************************************************************
	 * Event emitted when a mouse button is released.
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
	};

	/******************************************************************************************************************
	 * Event emitted when the mouse wheel is moved.
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
	 * Event emitted when a window hit-test occurs.
	 ******************************************************************************************************************/
	struct HitTestEvent {};

	/******************************************************************************************************************
	 * Union of window event types.
	 ******************************************************************************************************************/
	using WindowEvent = std::variant<WindowEnterEvent, WindowLeaveEvent, WindowShowEvent, WindowHideEvent,
									 WindowExposeEvent, WindowMotionEvent, WindowResizeEvent, WindowSizeChangeEvent,
									 WindowMinimizeEvent, WindowMaximizeEvent, WindowRestoreEvent, WindowGainFocusEvent,
									 WindowLoseFocusEvent, WindowCloseEvent, HitTestEvent>;

	/******************************************************************************************************************
	 * Sentinel value that signals that a ticker should tick forever.
	 ******************************************************************************************************************/
	[[maybe_unused]] inline constexpr std::uint32_t TICK_FOREVER{0};

	/******************************************************************************************************************
	 * Ticker that regularly emits events.
	 ******************************************************************************************************************/
	class Ticker {
	  public:
		/**************************************************************************************************************
		 * The event type emitted by tickers.
		 **************************************************************************************************************/
		struct Event {
			/**********************************************************************************************************
			 * The user-assigned ID attached to the ticker emitting the event.
			 **********************************************************************************************************/
			std::uint32_t id;
		};

		/**************************************************************************************************************
		 * Creates a ticker.
		 *
		 * @exception SDLError If creating the ticker failed.
		 *
		 * @param[in] id The ID that will be attached to events emitted by this ticker.
		 * @param[in] interval The interval between events.
		 * @param[in] nticks The number of ticks before halting or TICK_FOREVER.
		 **************************************************************************************************************/
		Ticker(std::int32_t id, MillisecondsD interval, std::uint32_t nticks);

		/// @private
		explicit Ticker(std::int32_t id, MillisecondsD interval, std::uint32_t nticks, bool sendDrawEvents);

		Ticker(Ticker&&) = delete;

		Ticker& operator=(Ticker&&) = delete;

		/**************************************************************************************************************
		 * Resets the ticker's interval.
		 *
		 * @param[in] interval The new interval between events.
		 **************************************************************************************************************/
		void resetInterval(MillisecondsD interval) noexcept;

	  private:
		int                        _id;
		bool                       _sendDrawEvents;
		std::int32_t               _eventID;
		std::uint32_t              _ticksLeft; // The number of ticks left before automatically halting or TICK_FOREVER.
		std::atomic<MillisecondsD> _interval;
		MillisecondsD              _accumulatedTimerError;

		static std::uint32_t callback(std::uint32_t interval, void* ptr) noexcept;

		friend class EventQueue;
	};

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
	};

	/******************************************************************************************************************
	 * Concept denoting a custom event.
	 *
	 * To fulfill this concept, a type must be constructible from CustomEventBase and convertible to it.
	 ******************************************************************************************************************/
	template <class T>
	concept CustomEventType = std::constructible_from<T, CustomEventBase> && std::is_convertible_v<T, CustomEventBase>;

	/******************************************************************************************************************
	 * Unified event type.
	 ******************************************************************************************************************/
	class Event {
	  public:
		/**************************************************************************************************************
		 * Constructs an event from a custom event base.
		 *
		 * @exception std::bad_alloc If allocating generic values fails.
		 *
		 * @param[in] custom The custom event base to convert into a generic event.
		 **************************************************************************************************************/
		Event(const CustomEventBase& custom);

		/**************************************************************************************************************
		 * Gets the type ID of the event.
		 *
		 * @return The type ID of the event.
		 **************************************************************************************************************/
		std::uint32_t type() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a key down event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator KeyDownEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a key up event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator KeyUpEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a text editing event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator TextEditEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a text input event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator TextInputEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a mouse motion event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator MouseMotionEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a mouse button down event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator MouseDownEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a mouse buttom up event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator MouseUpEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a mouse wheel event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator MouseWheelEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a window event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator WindowEvent() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a tick event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		operator Ticker::Event() const noexcept;

		/**************************************************************************************************************
		 * Converts the event into a key down event.
		 *
		 * The type must match, otherwise a failed assertion may be triggered.
		 **************************************************************************************************************/
		template <std::constructible_from<CustomEventBase> T> operator T() const noexcept;

	  private:
		alignas(8) std::byte _impl[56];

		Event() noexcept = default;

		CustomEventBase getCustomEventBase() const noexcept;

		friend class EventQueue;
	};

	/******************************************************************************************************************
	 * Sentinel value that signals that no draw events should be emitted.
	 ******************************************************************************************************************/
	[[maybe_unused]] inline constexpr std::uint32_t NO_DRAW_EVENTS{0};

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
		std::optional<Event> pollEvent() noexcept;

		/**************************************************************************************************************
		 * Gets an event from the event queue, or waits until one appears.
		 *
		 * @return The polled event.
		 **************************************************************************************************************/
		Event waitForEvent() noexcept;

		/**************************************************************************************************************
		 * Gets an event from the event queue, waiting until one appears or until a certain amount of time has passed.
		 *
		 * @param[in] timeout The maximum amount of time to wait before returning.
		 *
		 * @return The polled event, if one was found.
		 **************************************************************************************************************/
		std::optional<Event> waitForEventTimeout(std::chrono::milliseconds timeout) noexcept;

		/**************************************************************************************************************
		 * Sets the frequency at which draw events are sent at.
		 *
		 * @exception EventError If creating the draw ticker fails.
		 *
		 * @param[in] frequency The frequency of draw events, or NO_DRAW_EVENTS to stop sending draw events.
		 **************************************************************************************************************/
		void sendDrawEvents(unsigned int frequency);

		/**************************************************************************************************************
		 * Sets whether text input events should be sent to the event queue.
		 *
		 * @param[in] arg Whether text input events should be sent.
		 **************************************************************************************************************/
		void sendTextInputEvents(bool arg) noexcept;

		/**************************************************************************************************************
		 * Pushes an event to the queue.
		 *
		 * @exception EventError If pushing the event failed.
		 *
		 * @param[in] event The event to push.
		 **************************************************************************************************************/
		void pushEvent(const Event& event);

	  private:
		std::optional<Ticker> _drawTicker;

		EventQueue() noexcept = default;

		friend class Window;
	};

	/// @}
} // namespace tr

/// @cond

template <std::constructible_from<tr::CustomEventBase> T> tr::Event::operator T() const noexcept
{
	return T(getCustomEventBase());
}

/// @endcond
