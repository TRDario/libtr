module;
#include <SDL2/SDL.h>

export module tr:path;

import std;
import boost;

export namespace tr {
    struct ExeDirInitError : std::exception {
        virtual const char* what() const noexcept;
    };
    struct UserDirInitError : std::exception {
        virtual const char* what() const noexcept;
    };

    // Gets a path to the executable directory.
	std::filesystem::path getExeDir();
	// Gets a path to the user directory given an organization and application name.
	std::filesystem::path getUserDir(const char* org, const char* app);
}

export namespace std::filesystem {
    // Taken from https://stackoverflow.com/a/68734587.
    template <class CharT> void validate(boost::any& v, vector<basic_string<CharT>> const& s, path* p, int);
}

// IMPLEMENTATION

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

template <class CharT>
void std::filesystem::validate(boost::any& v, vector<basic_string<CharT>> const& s, path* p, int)
{
    assert(s.size() == 1);
    basic_stringstream<CharT> ss;

    for (auto& el : s) {
        ss << quoted(el);
    }

    path converted;
    ss >> noskipws >> converted;

    if (ss.peek(); !ss.eof()) {
        throw runtime_error("excess path characters");
    }

    v = std::move(converted);
}