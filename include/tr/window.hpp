#pragma once
#include "bitmap.hpp"
#include "display.hpp"
#include "event.hpp"
#include "framebuffer.hpp"
#include "graphics_context.hpp"
#include "mouse.hpp"
#include "sdl.hpp"
#include <magic_enum/magic_enum.hpp>

struct SDL_Window;

namespace tr {
	/** @defgroup system System
	 *  The primary window system.
	 *
	 *  An instance of Window must be created before any other functionality from this section can be used, with the
	 *  exception of @ref path.
	 *  @{
	 *  @}
	 */

	/** @ingroup system
	 *  @defgroup window Window
	 *  Main application window class and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown when opening a window fails.
	 ******************************************************************************************************************/
	struct WindowOpenError : std::runtime_error {
		using runtime_error::runtime_error;
	};

	/******************************************************************************************************************
	 * Error thrown when a window operation fails.
	 ******************************************************************************************************************/
	struct WindowError : SDLError {
		using SDLError::SDLError;
	};

	/******************************************************************************************************************
	 * Window modes.
	 ******************************************************************************************************************/
	enum class WindowMode : std::uint32_t {
		WINDOWED,
		FULLSCREEN = 0x1,
		BORDERLESS = 0x10'01
	};

	/******************************************************************************************************************
	 * Window flag bitmasks.
	 ******************************************************************************************************************/
	enum class WindowFlag : std::uint32_t {
		/**************************************************************************************************************
		 * Default flags.
		 **************************************************************************************************************/
		DEFAULT = 0x0,

		/**************************************************************************************************************
		 * The window has no decoration (topbar, etc.).
		 **************************************************************************************************************/
		BORDERLESS = 0x10,

		/**************************************************************************************************************
		 * The window is resizable.
		 **************************************************************************************************************/
		RESIZABLE = 0x20,

		/**************************************************************************************************************
		 * The window is minimized.
		 **************************************************************************************************************/
		MINIMIZED = 0x40,

		/**************************************************************************************************************
		 * The window is maximized.
		 **************************************************************************************************************/
		MAXIMIZED = 0x80,

		/**************************************************************************************************************
		 * The window has grabbed input focus.
		 **************************************************************************************************************/
		GRAB_INPUT = 0x2'00,

		/**************************************************************************************************************
		 * The window is always on top.
		 **************************************************************************************************************/
		ALWAYS_ON_TOP = 0x80'00
	};
	/// @cond IMPLEMENTATION
	DEFINE_BITMASK_OPERATORS(WindowFlag);
	/// @endcond

	/******************************************************************************************************************
	 * Flash operations.
	 ******************************************************************************************************************/
	enum class FlashOperation {
		/**************************************************************************************************************
		 * Cancel any existing window flashing, if there is any.
		 **************************************************************************************************************/
		CANCEL,

		/**************************************************************************************************************
		 * Flash the window briefly.
		 **************************************************************************************************************/
		BRIEF,

		/**************************************************************************************************************
		 * Flash the window continuously until the window regains focus.
		 **************************************************************************************************************/
		UNTIL_FOCUSED
	};

	/******************************************************************************************************************
	 * V-sync modes.
	 ******************************************************************************************************************/
	enum class VSync : std::int8_t {
		/**************************************************************************************************************
		 * Vsync is enabled, but late swaps happen immediately instead of waiting for the next retrace.
		 **************************************************************************************************************/
		ADAPTIVE = -1,

		/**************************************************************************************************************
		 * Vsync is disabled.
		 **************************************************************************************************************/
		DISABLED,

		/**************************************************************************************************************
		 * Vsync is enabled.
		 **************************************************************************************************************/
		ENABLED
	};

	/******************************************************************************************************************
	 * Sentinel for a centered position on the screen.
	 ******************************************************************************************************************/
	inline constexpr glm::ivec2 CENTERED_POS{0x2F'FF'00'00, 0x2F'FF'00'00};

	/******************************************************************************************************************
	 * The main application window.
	 *
	 * This class handles most of the basic functionality of the application, not just the window itself. Single
	 * instances of the EventQueue, Keyboard, Mouse and GraphicsContext classes belong to the window object.
	 *
	 * The Window class uses something akin to the singleton pattern. It is still your job to instantiate the window
	 * once (and only once!), after which it will stay active until its destructor is called, but this instance will be
	 * globally available through window(). Reopening the window after it has been closed is a valid action.
	 *
	 * Window is movable, but not copyable. A moved window is left in a state where another window can be moved into it,
	 * but is otherwise unusable.
	 ******************************************************************************************************************/
	class Window {
	  public:
		/**************************************************************************************************************
		 * Opens a windowed window.
		 *
		 * The window starts hidden and show() must be called.
		 *
		 * @exception WindowOpenError If opening the window fails.
		 *
		 * @param[in] title The title of the window.
		 * @param[in] size The size of the window in pixels.
		 * @param[in] pos The position of the window, offset to the top-left corner of the window in pixels.
		 *                Several special sentinels exist, such as CENTERED_POS, as well as DisplayInfo::centeredPos().
		 * @param[in] flags The flags of the window.
		 * @param[in] gfxProperties The properties of the window's graphics context.
		 **************************************************************************************************************/
		Window(const char* title, glm::ivec2 size, glm::ivec2 pos = CENTERED_POS,
			   WindowFlag flags = WindowFlag::DEFAULT, const GraphicsProperties& gfxProperties = {});

		/**************************************************************************************************************
		 * Opens a borderless fullscreen window.
		 *
		 * The window starts hidden and show() must be called.
		 *
		 * @exception WindowOpenError If opening the window fails.
		 *
		 * @param[in] title The title of the window.
		 * @param[in] display The display to put the window on.
		 * @param[in] flags The flags of the window.
		 * @param[in] gfxProperties The properties of the window's graphics context.
		 **************************************************************************************************************/
		Window(const char* title, DisplayInfo display = tr::display(), WindowFlag flags = WindowFlag::DEFAULT,
			   const GraphicsProperties& gfxProperties = {});

		/**************************************************************************************************************
		 * Opens a fullscreen window.
		 *
		 * The window starts hidden and show() must be called.
		 *
		 * @exception WindowOpenError If opening the window fails.
		 *
		 * @param[in] title The title of the window.
		 * @param[in] dmode The display mode to use.
		 * @param[in] display The display to put the window on.
		 * @param[in] flags The flags of the window.
		 * @param[in] gfxProperties The properties of the window's graphics context.
		 **************************************************************************************************************/
		Window(const char* title, const DisplayMode& dmode, DisplayInfo display = tr::display(),
			   WindowFlag flags = WindowFlag::DEFAULT, const GraphicsProperties& gfxProperties = {});

		/**************************************************************************************************************
		 * Gets the title of the window.
		 *
		 * @return A C-string containing the title of the window.
		 **************************************************************************************************************/
		const char* title() const noexcept;

		/**************************************************************************************************************
		 * Sets the title of the window.
		 *
		 * @param[in] title The new title of the window.
		 **************************************************************************************************************/
		void setTitle(const char* title) noexcept;

		/**************************************************************************************************************
		 * Sets the title of the window.
		 *
		 * @param[in] title The new title of the window.
		 **************************************************************************************************************/
		void setTitle(const std::string& title) noexcept;

		/**************************************************************************************************************
		 * Sets the icon of the window.
		 *
		 * @param[in] bitmap The new window icon.
		 **************************************************************************************************************/
		void setIcon(const Bitmap& bitmap) noexcept;

		/**************************************************************************************************************
		 * Sets the icon of the window.
		 *
		 * @param[in] view The new window icon.
		 **************************************************************************************************************/
		void setIcon(const BitmapView& view) noexcept;

		/**************************************************************************************************************
		 * Gets the size of the window.
		 *
		 * @return The size of the window in pixels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

		/**************************************************************************************************************
		 * Sets the size of the window.
		 *
		 * @param[in] size The new size of the window in pixels.
		 **************************************************************************************************************/
		void setSize(glm::ivec2 size) noexcept;

		/**************************************************************************************************************
		 * Gets the window's fullscreen display mode.
		 *
		 * @return The window's fullscreen display mode, or std::nullopt if the window isn't fullscreen.
		 **************************************************************************************************************/
		std::optional<DisplayMode> fullscreenMode() const noexcept;

		/**************************************************************************************************************
		 * Sets the window's fullscreen display mode.
		 *
		 * This function sets the window to fullscreen if it isn't already.
		 *
		 * @exception WindowError If setting the fullscreen mode fails.
		 *
		 * @param[in] dmode The new fullscreen mode.
		 **************************************************************************************************************/
		void setFullscreenMode(const DisplayMode& dmode);

		/**************************************************************************************************************
		 * Gets the window's window mode.
		 *
		 * @return The window's display mode.
		 **************************************************************************************************************/
		WindowMode windowMode() const noexcept;

		/**************************************************************************************************************
		 * Sets the window's window mode.
		 *
		 * @exception WindowError If setting the window mode fails.
		 *
		 * @param[in] mode The new display mode.
		 **************************************************************************************************************/
		void setWindowMode(WindowMode mode);

		/**************************************************************************************************************
		 * Gets whether the window is resizable.
		 *
		 * @return True if the window is resizable, and false otherwise.
		 **************************************************************************************************************/
		bool resizable() const noexcept;

		/**************************************************************************************************************
		 * Sets whether the window is resizable.
		 *
		 * @param[in] resizable Whether the window should be resizable.
		 **************************************************************************************************************/
		void setResizable(bool resizable) noexcept;

		/**************************************************************************************************************
		 * Gets the minimum resizable size of the window.
		 *
		 * @return The minimum resizable size of the window.
		 **************************************************************************************************************/
		glm::ivec2 minSize() const noexcept;

		/**************************************************************************************************************
		 * Sets the minimum resizable size of the window.
		 *
		 * @param[in] minSize The minimum resiable size of the window.
		 **************************************************************************************************************/
		void setMinSize(glm::ivec2 minSize) noexcept;

		/**************************************************************************************************************
		 * Gets the maximum resizable size of the window.
		 *
		 * @return The maximum resizable size of the window.
		 **************************************************************************************************************/
		glm::ivec2 maxSize() const noexcept;

		/**************************************************************************************************************
		 * Sets the maximum resizable size of the window.
		 *
		 * @param[in] maxSize The maximum resiable size of the window.
		 **************************************************************************************************************/
		void setMaxSize(glm::ivec2 maxSize) noexcept;

		/**************************************************************************************************************
		 * Gets the position of the window relative to the top-left corner of the display.
		 *
		 * @return The position of the window relative to the top-left corner of the display in pixels.
		 **************************************************************************************************************/
		glm::ivec2 position() const noexcept;

		/**************************************************************************************************************
		 * Sets the position of the window relative to the top-left corner of the display.
		 *
		 * @param[in] pos The new position of the window relative to the top-left corner of the display in pixels.
		 **************************************************************************************************************/
		void setPosition(glm::ivec2 pos) noexcept;

		/**************************************************************************************************************
		 * Gets whether the window is bordered.
		 *
		 * @return True if the window is bordered, and false otherwise.
		 **************************************************************************************************************/
		bool bordered() const noexcept;

		/**************************************************************************************************************
		 * Sets whether the mouse is bordered.
		 *
		 * @param[in] bordered Whether the mouse should be bordered.
		 **************************************************************************************************************/
		void setBordered(bool bordered) noexcept;

		/**************************************************************************************************************
		 * Gets whether the window is being shown.
		 *
		 * @return True if the window is being shown, and false otherwise.
		 **************************************************************************************************************/
		bool shown() const noexcept;

		/**************************************************************************************************************
		 * Unhides the window.
		 **************************************************************************************************************/
		void show() noexcept;

		/**************************************************************************************************************
		 * Hides the window.
		 **************************************************************************************************************/
		void hide() noexcept;

		/**************************************************************************************************************
		 * Gets whether the window is maximized.
		 *
		 * @return True if the window is maximized, and false otherwise.
		 **************************************************************************************************************/
		bool maximized() const noexcept;

		/**************************************************************************************************************
		 * Maximizes the window.
		 **************************************************************************************************************/
		void maximize() noexcept;

		/**************************************************************************************************************
		 * Restores the window back to its previous size.
		 **************************************************************************************************************/
		void restore() noexcept;

		/**************************************************************************************************************
		 * Gets whether the window is minimized.
		 *
		 * @return True if the window is minimized, and false otherwise.
		 **************************************************************************************************************/
		bool minimized() const noexcept;

		/**************************************************************************************************************
		 * Minimizes the window.
		 **************************************************************************************************************/
		void minimize() noexcept;

		/**************************************************************************************************************
		 * Gets whether the window has input focus.
		 *
		 * @return True if the window has input focus, and false otherwise.
		 **************************************************************************************************************/
		bool hasFocus() const noexcept;

		/**************************************************************************************************************
		 * Raises the window above the rest and gives it input focus.
		 **************************************************************************************************************/
		void raise() noexcept;

		/**************************************************************************************************************
		 * Gets whether the mouse is grabbed and confined to this window.
		 *
		 * @return True if the mouse is grabbed, and false otherwise.
		 **************************************************************************************************************/
		bool mouseGrabbed() const noexcept;

		/**************************************************************************************************************
		 * Sets whether the mouse is grabbed and confined to this window.
		 *
		 * @param[in] grab Whether the mouse should be grabbed.
		 **************************************************************************************************************/
		void setMouseGrab(bool grab) noexcept;

		/**************************************************************************************************************
		 * Gets the area the mouse is confined to when this window has focus.
		 *
		 * @return The rectangle to which the mouse is confined to or std::nullopt if it is not set.
		 **************************************************************************************************************/
		std::optional<RectI2> mouseConfines() const noexcept;

		/**************************************************************************************************************
		 * Sets the area the mouse is confined to when this window has focus.
		 *
		 * @param[in] rect The confinement rectangle.
		 **************************************************************************************************************/
		void setMouseConfines(const RectI2& rect) noexcept;

		/**************************************************************************************************************
		 * Resets the area the mouse is confined to.
		 **************************************************************************************************************/
		void resetMouseConfines() noexcept;

		/**************************************************************************************************************
		 * Gets whether the window is always on top.
		 *
		 * @return True if the window is always on top, or false otherwise.
		 **************************************************************************************************************/
		bool alwaysOnTop() const noexcept;

		/**************************************************************************************************************
		 * Sets whether the window is always on top.
		 *
		 * @param[in] alwaysOnTop Whether the window should be always on top.
		 **************************************************************************************************************/
		void setAlwaysOnTop(bool alwaysOnTop) noexcept;

		/**************************************************************************************************************
		 * Flashes the window to get the user's attention.
		 *
		 * @param[in] operation The type of flashing to perform.
		 **************************************************************************************************************/
		void flash(FlashOperation operation) noexcept;

		/**************************************************************************************************************
		 * Gets the opacity of the window.
		 *
		 * @return The opacity of the window [0.0, 1.0].
		 **************************************************************************************************************/
		float opacity() const noexcept;

		/**************************************************************************************************************
		 * Sets the opacity of the window.
		 *
		 * @param opacity The opacity of the window [0.0, 1.0].
		 **************************************************************************************************************/
		void setOpacity(float opacity) noexcept;

		/**************************************************************************************************************
		 * Gets the window's V-sync mode.
		 *
		 * @return The window's V-sync mode.
		 **************************************************************************************************************/
		VSync vsync() const noexcept;

		/**************************************************************************************************************
		 * Sets the window's V-sync mode.
		 *
		 * If setting the V-sync mode failed and the parameter is ADAPTIVE, regular V-sync will be used.
		 *
		 * @param[in] vsync The new V-sync mode.
		 **************************************************************************************************************/
		void setVSync(VSync vsync) noexcept;

		/**************************************************************************************************************
		 * Gets the window's graphics context.
		 *
		 * @return A mutable reference to the window's graphics context.
		 **************************************************************************************************************/
		GraphicsContext& graphics() noexcept;

		/**************************************************************************************************************
		 * Gets the window's graphics context.
		 *
		 * @return An immutable reference to the window's graphics context.
		 **************************************************************************************************************/
		const GraphicsContext& graphics() const noexcept;

		/**************************************************************************************************************
		 * Gets the window's backbuffer.
		 *
		 * @return A mutable reference to the window's backbuffer.
		 **************************************************************************************************************/
		Backbuffer& backbuffer() noexcept;

		/**************************************************************************************************************
		 * Gets the window's backbuffer.
		 *
		 * @return An immutable reference to the window's backbuffer.
		 **************************************************************************************************************/
		const Backbuffer& backbuffer() const noexcept;

		/**************************************************************************************************************
		 * Gets the window's event queue.
		 *
		 * @return A mutable reference to the window's event queue.
		 **************************************************************************************************************/
		EventQueue& events() noexcept;

		/**************************************************************************************************************
		 * Gets the window's event queue.
		 *
		 * @return An immutable reference to the window's event queue.
		 **************************************************************************************************************/
		const EventQueue& events() const noexcept;

		/**************************************************************************************************************
		 * Gets access to the keyboard.
		 *
		 * @return An immutable reference to the keyboard.
		 **************************************************************************************************************/
		const Keyboard& keyboard() const noexcept;

		/**************************************************************************************************************
		 * Gets access to the mouse.
		 *
		 * @return A mutable reference to the mouse.
		 **************************************************************************************************************/
		Mouse& mouse() noexcept;

		/**************************************************************************************************************
		 * Gets access to the mouse.
		 *
		 * @return An immutable reference to the mouse.
		 **************************************************************************************************************/
		const Mouse& mouse() const noexcept;

	  private:
		/// @cond IMPLEMENTATION
		struct SDLDeleter {
			void operator()(bool) const noexcept;
		};
		struct WindowDeleter {
			void operator()(SDL_Window* ptr) const noexcept;
		};
		/// @endcond

		Handle<bool, false, SDLDeleter>            _sdl{true};
		std::unique_ptr<SDL_Window, WindowDeleter> _impl;
		GraphicsContext                            _glContext;
		Backbuffer                                 _backbuffer;
		EventQueue                                 _eventQueue;
#ifdef _MSC_VER
		[[msvc::no_unique_address]] Keyboard _keyboard;
		[[msvc::no_unique_address]] Mouse    _mouse;
#else
		[[no_unique_address]] Keyboard _keyboard;
		[[no_unique_address]] Mouse    _mouse;
#endif

		friend class GraphicsContext;
	};

	/******************************************************************************************************************
	 * Gets whether the window was opened.
	 *
	 * @return True if the window was opened, and false otherwise.
	 ******************************************************************************************************************/
	bool windowOpened() noexcept;

	/******************************************************************************************************************
	 * Gets a reference to the window. This function cannot be called if the window wasn't opened.
	 *
	 * @return A reference to the window.
	 ******************************************************************************************************************/
	Window& window() noexcept;

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <> struct magic_enum::customize::enum_range<tr::WindowMode> {
	static constexpr int min = int(tr::WindowMode::WINDOWED);
	static constexpr int max = int(tr::WindowMode::BORDERLESS);
};

/// @endcond
