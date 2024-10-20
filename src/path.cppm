/**
 * @file path.cppm
 * @brief Provides common directory accessors.
 */

module;
#include <SDL2/SDL.h>

export module tr:path;

import std;

export namespace tr {
    /******************************************************************************************************************
    * Error in getting the executable directory path.
    ******************************************************************************************************************/
    struct ExeDirInitError : std::exception {
        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
        virtual const char* what() const noexcept;
    };

    /******************************************************************************************************************
    * Error in getting the executable directory path.
    ******************************************************************************************************************/
    struct UserDirInitError : std::exception {
        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
        virtual const char* what() const noexcept;
    };


    /******************************************************************************************************************
    * Gets a path to the executable directory.
    *
    * This function should only be called once at the beginning of the program as it may be relatively expensive.
    *
    * @exception ExeDirInitError If getting the path failed.
    * @exception std::bad_alloc If allocating the path failed.
    *
    * @return A fath to the executable directory.
    ******************************************************************************************************************/
	std::filesystem::path getExeDir();
	
    /******************************************************************************************************************
    * Gets a path to a safe user directory where data can be stored.
    *
    * This function should only be called once at the beginning of the program as it may be relatively expensive.
    *
    * @exception UserDirInitError If getting the path failed.
    * @exception std::bad_alloc If allocating the path failed.
    *
    * @param org The organization name, used so that all applications under this organization share a folder.
    * @param app The application name, used to that a specific subdirectory be made for the application.
    *
    * @return A path to the executable directory.
    ******************************************************************************************************************/
	std::filesystem::path getUserDir(const char* org, const char* app);
}

/// @cond IMPLEMENTATION

const char* tr::ExeDirInitError::what() const noexcept
{
    return "Failed to initialize the executable directory base path.";
}

const char* tr::UserDirInitError::what() const noexcept
{
    return "Failed to initialize the user directory base path.";
}

std::filesystem::path tr::getExeDir()
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    std::unique_ptr<char[], decltype(&SDL_free)> cExedir { SDL_GetBasePath(), SDL_free };
    if (cExedir == nullptr) {
        throw ExeDirInitError {};
    }
    std::filesystem::path exedir { cExedir.get() };
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    return exedir;
}

std::filesystem::path tr::getUserDir(const char* org, const char* app)
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    std::unique_ptr<char[], decltype(&SDL_free)> cUserdir { SDL_GetPrefPath(org, app), SDL_free };
    if (cUserdir == nullptr) {
        throw UserDirInitError {};
    }
    std::filesystem::path userdir { cUserdir.get() };
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    return userdir;
}

/// @endcond