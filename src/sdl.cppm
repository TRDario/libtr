/**
 * @file sdl.cppm
 * @brief Provides an SDL library RAII wrapper and related functionality.
 */

module;
#include <SDL2/SDL.h>

export module tr:sdl;

import std;
import :handle;

export namespace tr {
    /******************************************************************************************************************
	 * OpenGL attributes used by SDL.
	 ******************************************************************************************************************/
    struct GLAttrs {
        /**************************************************************************************************************
         * Whether a debug context should be used.
         **************************************************************************************************************/
        bool debugContext = false;

        /**************************************************************************************************************
         * The number of bits to use for backbuffer depth.
         **************************************************************************************************************/
        std::uint8_t  depthBits = 0;

        /**************************************************************************************************************
         * The number of bits to use for backbuffer stencil.
         **************************************************************************************************************/
        std::uint8_t  stencilBits = 0;

        /**************************************************************************************************************
         * The number of samples used around a pixel for multisampled anti-aliasing.
         **************************************************************************************************************/
        std::uint8_t  multisamples = 0;
    };


    /******************************************************************************************************************
	 * Library version information.
     *
     * This struct is printable via std::format.
	 ******************************************************************************************************************/
    struct Version {
        /**************************************************************************************************************
         * The major (first) version number.
         **************************************************************************************************************/
        std::uint8_t major;

        /**************************************************************************************************************
         * The minor (second) version number.
         **************************************************************************************************************/
        std::uint8_t minor;

        /**************************************************************************************************************
         * The patch (third) version number.
         **************************************************************************************************************/
        std::uint8_t patch;
    };


    /******************************************************************************************************************
	 * Error thrown by an SDL function failing.
	 ******************************************************************************************************************/
    struct SDLError : std::runtime_error {
        /**************************************************************************************************************
        * Constructs an SDL error.
        *
        * @exception std::bad_alloc If allocating the error string failed.
        *
        * @param message The error message.
        **************************************************************************************************************/
        SDLError(std::string_view message);
    };


    /******************************************************************************************************************
	 * SDL library RAII wrapper.
	 ******************************************************************************************************************/
    class SDL {
    public:
        /**************************************************************************************************************
         * Initializes the SDL library.
         *
         * @exception SDLError If initializing the library failed.
         *
         * @param glAttrs Attributes used for the OpenGL context.
         **************************************************************************************************************/
        SDL(const GLAttrs& glAttrs = {});


        /**************************************************************************************************************
         * Gets the version of the SDL library.
         *
         * @return A structure containing SDL version info.
         **************************************************************************************************************/
        Version version() const noexcept;

        /**************************************************************************************************************
         * Gets the revision string of the SDL library.
         *
         * @return A C-string containing SDL revision info.
         **************************************************************************************************************/
        const char* revision() const noexcept;

        /**************************************************************************************************************
         * Gets the platform string.
         *
         * @return A C-string containing platform info.
         **************************************************************************************************************/
        const char* platform() const noexcept;

        /**************************************************************************************************************
         * Gets the amount of system RAM.
         *
         * @return The amount of system RAM in megabytes.
         **************************************************************************************************************/
        int systemRAM() const noexcept;

        /**************************************************************************************************************
         * Gets the number of system cores.
         *
         * @return The number of system cores.
         **************************************************************************************************************/
        int systemCores() const noexcept;
    private:
        Handle<bool, false, decltype([] (bool) { SDL_QuitSubSystem(SDL_INIT_VIDEO); SDL_Quit(); })> _handle { true };
    };
}

/// @cond IMPLEMENTATION

export namespace std {
    template <> struct formatter<tr::Version> : formatter<string> {
        auto format(tr::Version v, format_context& ctx) const {
            return formatter<string>::format(std::format("{:d}.{:d}.{:d}", v.major, v.minor, v.patch), ctx);
        }
    };
}

namespace tr {
    void setSDLGLAttributes(const GLAttrs& attrs) noexcept;
    void suppressUnsupportedEvents() noexcept;
}

void tr::setSDLGLAttributes(const GLAttrs& attrs) noexcept
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | (attrs.debugContext ? SDL_GL_CONTEXT_DEBUG_FLAG : 0));
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, attrs.depthBits);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, attrs.stencilBits);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, bool(attrs.multisamples));
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, attrs.multisamples);
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

tr::SDLError::SDLError(std::string_view message)
    : runtime_error { std::format("{} ({})", message, SDL_GetError()) }
{}

tr::SDL::SDL(const GLAttrs& glAttrs)
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        SDL_Quit();
        throw SDLError { "Failed to initialize SDL" };
    }
    setSDLGLAttributes(glAttrs);
    suppressUnsupportedEvents();
}

tr::Version tr::SDL::version() const noexcept
{
    SDL_version version;
    SDL_GetVersion(&version);
    return std::bit_cast<Version>(version);
}

const char* tr::SDL::revision() const noexcept
{
    return SDL_GetRevision();
}

const char* tr::SDL::platform() const noexcept
{
    return SDL_GetPlatform();
}

int tr::SDL::systemRAM() const noexcept
{
    return SDL_GetSystemRAM();
}

int tr::SDL::systemCores() const noexcept
{
    return SDL_GetCPUCount();
}

/// @endcond