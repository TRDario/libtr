#include "../include/tr/sdl.hpp"
#include <SDL2/SDL.h>

namespace tr {
    void setSDLGLAttributes(const GLAttrs& attrs) noexcept;
    void suppressUnsupportedEvents() noexcept;
}


void tr::setSDLGLAttributes(const GLAttrs& attrs) noexcept
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
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

void tr::SDL::Deleter::operator()(bool) const noexcept {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
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