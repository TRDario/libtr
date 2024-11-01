/**
 * @file window.hpp
 * @brief Provides application window functionality.
 */

#pragma once
#include "dependencies/magic_enum.hpp"
#include "bitmap.hpp"
#include "display.hpp"

struct SDL_Window;

namespace tr {
	/******************************************************************************************************************
	 * Error thrown when opening a window failed.
	 ******************************************************************************************************************/
	struct WindowOpenError : std::runtime_error {
		using runtime_error::runtime_error;
	};

	/******************************************************************************************************************
	 * Error thrown on a window operation failure.
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
		BORDERLESS = 0x1001
	};

	/******************************************************************************************************************
	 * Window flag bitmasks.
	 ******************************************************************************************************************/
	enum class WindowFlag : std::uint32_t {
		/**************************************************************************************************************
		 * Default flags.
		 **************************************************************************************************************/
		DEFAULT       = 0x0,

		/**************************************************************************************************************
		 * The window is visible.
		 **************************************************************************************************************/
		SHOWN         = 0x4,

		/**************************************************************************************************************
		 * The window is not visible.
		 **************************************************************************************************************/
		HIDDEN        = 0x8,

		/**************************************************************************************************************
		 * The window has no decoration (topbar, etc.).
		 **************************************************************************************************************/
		BORDERLESS    = 0x10,

		/**************************************************************************************************************
		 * The window is resizable.
		 **************************************************************************************************************/
		RESIZABLE     = 0x20,

		/**************************************************************************************************************
		 * The window is minimized.
		 **************************************************************************************************************/
		MINIMIZED     = 0x40,

		/**************************************************************************************************************
		 * The window is maximized.
		 **************************************************************************************************************/
		MAXIMIZED     = 0x80,

		/**************************************************************************************************************
		 * The window has grabbed input focus.
		 **************************************************************************************************************/
		GRAB_INPUT    = 0x200,

		/**************************************************************************************************************
		 * The window is always on top.
		 **************************************************************************************************************/
		ALWAYS_ON_TOP = 0x8000
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
	 * Hit test results.
	 ******************************************************************************************************************/
	enum class HitTestResult {
		/**************************************************************************************************************
		 * Region is normal and has no special properties.
		 **************************************************************************************************************/
		NORMAL,

		/**************************************************************************************************************
		 * Region can drag entire window.
		 **************************************************************************************************************/
		DRAGGABLE,

		/**************************************************************************************************************
		 * Region can resize from the top left of the window.
		 **************************************************************************************************************/
		TOP,       // Region can resize from the top of the window.

		/**************************************************************************************************************
		 * Region can resize from the top right of the window.
		 **************************************************************************************************************/
		TR,

		/**************************************************************************************************************
		 * Region can resize from the right of the window.
		 **************************************************************************************************************/
		RIGHT,

		/**************************************************************************************************************
		 * Region can resize from the bottom right of the window.
		 **************************************************************************************************************/
		BR,

		/**************************************************************************************************************
		 * Region can resize from the bottom of the window.
		 **************************************************************************************************************/
		BOTTOM,

		/**************************************************************************************************************
		 * Region can resize from the bottom left of the window.
		 **************************************************************************************************************/
		BL,

		/**************************************************************************************************************
		 * Region can resize from the left of the window.
		 **************************************************************************************************************/
		LEFT
	};


	/******************************************************************************************************************
	 * Hit test callback signature type.
	 ******************************************************************************************************************/
	using HitTestCB = std::function<HitTestResult(glm::ivec2)>;


	/******************************************************************************************************************
	 * Sentinel for a centered position on the screen.
	 ******************************************************************************************************************/
	inline constexpr glm::ivec2 CENTERED_POS { 0x2FFF0000, 0x2FFF0000 };


	/******************************************************************************************************************
	 * Non-owning view over a window.
	 ******************************************************************************************************************/
	class WindowView {
	public:
		/// @private
		explicit WindowView(SDL_Window* window) noexcept;


		/**************************************************************************************************************
		 * Gets the title of the window.
		 *
		 * @return A C-string containing the title of the window.
		 **************************************************************************************************************/
		const char* title() const noexcept;

		/**************************************************************************************************************
		 * Sets the title of the window.
		 *
		 * @param title The new title of the window.
		 **************************************************************************************************************/
		void setTitle(const char* title) const noexcept;


		/**************************************************************************************************************
		 * Sets the icon of the window.
		 *
		 * @param bitmap The new window icon.
		 **************************************************************************************************************/
		void setIcon(const Bitmap& bitmap) const noexcept;


		/**************************************************************************************************************
		 * Gets the size of the window.
		 *
		 * @return The size of the window in pixels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;
		
		/**************************************************************************************************************
		 * Sets the size of the window.
		 *
		 * @param size The new size of the window in pixels.
		 **************************************************************************************************************/
		void setSize(glm::ivec2 size) const noexcept;


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
		 * @exception WindowError If setting the fullscreen mode failed.
		 *
		 * @param dmode The new fullscreen mode.
		 **************************************************************************************************************/
		void setFullscreenMode(const DisplayMode& dmode) const;


		/**************************************************************************************************************
		 * Gets the window's window mode.
		 *
		 * @return The window's display mode.
		 **************************************************************************************************************/
		WindowMode windowMode() const noexcept;
		
		/**************************************************************************************************************
		 * Sets the window's window mode.
		 *
		 * @exception WindowError If setting the window mode failed.
		 *
		 * @param mode The new display mode.
		 **************************************************************************************************************/
		void setWindowMode(WindowMode mode) const;


		/**************************************************************************************************************
		 * Gets whether the window is resizable.
		 *
		 * @return True if the window is resizable, and false otherwise.
		 **************************************************************************************************************/
		bool resizable() const noexcept;

		/**************************************************************************************************************
		 * Sets whether the window is resizable.
		 *
		 * @param resizable Whether the window should be resizable.
		 **************************************************************************************************************/
		void setResizable(bool resizable) const noexcept;


		/**************************************************************************************************************
		 * Gets the minimum resizable size of the window.
		 *
		 * @return The minimum resizable size of the window.
		 **************************************************************************************************************/
		glm::ivec2 minSize() const noexcept;

		/**************************************************************************************************************
		 * Sets the minimum resizable size of the window.
		 *
		 * @param minSize The minimum resiable size of the window.
		 **************************************************************************************************************/
		void setMinSize(glm::ivec2 minSize) const noexcept;


		/**************************************************************************************************************
		 * Gets the maximum resizable size of the window.
		 *
		 * @return The maximum resizable size of the window.
		 **************************************************************************************************************/
		glm::ivec2 maxSize() const noexcept;

		/**************************************************************************************************************
		 * Sets the maximum resizable size of the window.
		 *
		 * @param maxSize The maximum resiable size of the window.
		 **************************************************************************************************************/
		void setMaxSize(glm::ivec2 maxSize) const noexcept;


		/**************************************************************************************************************
		 * Gets the position of the window relative to the top-left corner of the display.
		 *
		 * @return The position of the window relative to the top-left corner of the display in pixels.
		 **************************************************************************************************************/
		glm::ivec2 pos() const noexcept;

		/**************************************************************************************************************
		 * Sets the position of the window relative to the top-left corner of the display.
		 *
		 * @param pos The new position of the window relative to the top-left corner of the display in pixels.
		 **************************************************************************************************************/
		void setPos(glm::ivec2 pos) const noexcept;


		/**************************************************************************************************************
		 * Gets whether the window is bordered.
		 *
		 * @return True if the window is bordered, and false otherwise.
		 **************************************************************************************************************/
		bool bordered() const noexcept;

		/**************************************************************************************************************
		 * Sets whether the mouse is bordered.
		 *
		 * @param bordered Whether the mouse should be bordered.
		 **************************************************************************************************************/
		void setBordered(bool bordered) const noexcept;


		/**************************************************************************************************************
		 * Gets whether the window is being shown.
		 *
		 * @return True if the window is being shown, and false otherwise.
		 **************************************************************************************************************/
		bool shown() const noexcept;

		/**************************************************************************************************************
		 * Unhides the window.
		 **************************************************************************************************************/
		void show() const noexcept;

		/**************************************************************************************************************
		 * Hides the window.
		 **************************************************************************************************************/
		void hide() const noexcept;


		/**************************************************************************************************************
		 * Gets whether the window is maximized.
		 *
		 * @return True if the window is maximized, and false otherwise.
		 **************************************************************************************************************/
		bool maximized() const noexcept;

		/**************************************************************************************************************
		 * Maximizes the window.
		 **************************************************************************************************************/
		void maximize() const noexcept;

		/**************************************************************************************************************
		 * Restores the window back to its previous size.
		 **************************************************************************************************************/
		void restore() const noexcept;


		/**************************************************************************************************************
		 * Gets whether the window is minimized.
		 *
		 * @return True if the window is minimized, and false otherwise.
		 **************************************************************************************************************/
		bool minimized() const noexcept;

		/**************************************************************************************************************
		 * Minimizes the window.
		 **************************************************************************************************************/
		void minimize() const noexcept;


		/**************************************************************************************************************
		 * Gets whether the window has input focus.
		 *
		 * @return True if the window has input focus, and false otherwise.
		 **************************************************************************************************************/
		bool hasFocus() const noexcept;

		/**************************************************************************************************************
		 * Raises the window above the rest and gives it input focus.
		 **************************************************************************************************************/
		void raise() const noexcept;


		/**************************************************************************************************************
		 * Gets whether the mouse is grabbed and confined to this window.
		 *
		 * @return True if the mouse is grabbed, and false otherwise.
		 **************************************************************************************************************/
		bool mouseGrabbed() const noexcept;

		/**************************************************************************************************************
		 * Sets whether the mouse is grabbed and confined to this window.
		 *
		 * @param grab Whether the mouse should be grabbed.
		 **************************************************************************************************************/
		void setMouseGrab(bool grab) const noexcept;


		/**************************************************************************************************************
		 * Gets the area the mouse is confined to when this window has focus.
		 *
		 * @return The rectangle to which the mouse is confined to or std::nullopt if it is not set.
		 **************************************************************************************************************/
		std::optional<RectI2> mouseConfines() const noexcept;

		/**************************************************************************************************************
		 * Sets the area the mouse is confined to when this window has focus.
		 *
		 * @param rect The confinement rectangle.
		 **************************************************************************************************************/
		void setMouseConfines(const RectI2& rect) const noexcept;

		/**************************************************************************************************************
		 * Resets the area the mouse is confined to.
		 **************************************************************************************************************/
		void resetMouseConfines() const noexcept;


		/**************************************************************************************************************
		 * Gets whether the window is always on top.
		 *
		 * @return True if the window is always on top, or false otherwise.
		 **************************************************************************************************************/
		bool alwaysOnTop() const noexcept;

		/**************************************************************************************************************
		 * Sets whether the window is always on top.
		 *
		 * @param alwaysOnTop Whether the window should be always on top.
		 **************************************************************************************************************/
		void setAlwaysOnTop(bool alwaysOnTop) const noexcept;


		/**************************************************************************************************************
		 * Flashes the window to get the user's attention.
		 *
		 * @exception WindowError If flashing the window failed.
		 *
		 * @param operation The type of flashing to perform.
		 **************************************************************************************************************/
		void flash(FlashOperation operation) const;


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
		void setOpacity(float opacity) const noexcept;


		/**************************************************************************************************************
		 * Swaps the display's front and back buffers.
		 **************************************************************************************************************/
		void swap() const noexcept;
	protected:
		/// @private
		// The underlying SDL window pointer.
		SDL_Window* _impl;

		friend class Event;
	};

	/**************************************************************************************************************
	 * Application window.
	 *
	 * No instances of this class can be created before initializing SDL.
	 **************************************************************************************************************/
	class Window : public WindowView {
	public:
		/**************************************************************************************************************
		 * Opens a window.
		 *
		 * @exception WindowOpenError If creating the window failed.
		 *
		 * @param title The title of the window.
		 * @param size The size of the window in pixels.
		 * @param pos The position of the window, offset to the top-left corner of the window in pixels.
		 *            Several special sentinels exist, such as CENTERED_POS, as well as DisplayInfo::centeredPos().
		 * @param flags The flags of the window.
		 **************************************************************************************************************/
		Window(const char* title, glm::ivec2 size, glm::ivec2 pos = CENTERED_POS, WindowFlag flags = WindowFlag::DEFAULT);
		
		/**************************************************************************************************************
		 * Opens a borderless fullscreen window.
		 *
		 * @exception WindowOpenError If creating the window failed.
		 *
		 * @param title The title of the window.
		 * @param display The display to put the window on.
		 * @param flags The flags of the window.
		 **************************************************************************************************************/
		Window(const char* title, DisplayInfo display = DEFAULT_DISPLAY, WindowFlag flags = WindowFlag::DEFAULT);
		
		/**************************************************************************************************************
		 * Opens a fullscreen window.
		 *
		 * @exception WindowOpenError If creating the window failed.
		 * @exception WindowError If setting the fullscreen mode failed.
		 *
		 * @param title The title of the window.
		 * @param dmode The display mode to use.
		 * @param display The display to put the window on.
		 * @param flags The flags of the window.
		 **************************************************************************************************************/
		Window(const char* title, const DisplayMode& dmode, DisplayInfo display = DEFAULT_DISPLAY, WindowFlag flags = WindowFlag::DEFAULT);

		/**************************************************************************************************************
		 * Move-constructs a window.
		 *
		 * The assigned-from window will be left in a state on which operations are not allowed to be performed.
		 **************************************************************************************************************/
		Window(Window&&) noexcept;

		/**************************************************************************************************************
		 * Destroys the window.
		 *
		 * Using any views of this window after destruction is undefined behavior.
		 **************************************************************************************************************/
		~Window() noexcept;


		/**************************************************************************************************************
		 * Move-assigns the window.
		 *
		 * The assigned-from window will be left in a state on which operations are not allowed to be performed.
		 *
		 * @return A reference to the assigned window.
		 **************************************************************************************************************/
		Window& operator=(Window&&) noexcept;


		/**************************************************************************************************************
		 * Sets a hit-test callback that designates pieces of the window as special.
		 *
		 * @exception WindowError If setting the hit-test failed.
		 *
		 * @param hitTestCB The funciton to use for the callback.
		 **************************************************************************************************************/
		void setHitTest(HitTestCB hitTestCB);
	private:
		std::optional<HitTestCB> _hitTest;

		friend class GLContext;
	};
}

/// @cond IMPLEMENTATION

template <>
struct magic_enum::customize::enum_range<tr::WindowMode> {
  	static constexpr int min = int(tr::WindowMode::WINDOWED);
  	static constexpr int max = int(tr::WindowMode::BORDERLESS);
};

/// @endcond
