#include "../include/tr/path.hpp"
#include <SDL2/SDL.h>

std::filesystem::path tr::getExeDir()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	std::unique_ptr<char[], decltype([](void* ptr) { SDL_free(ptr); })> cExedir{SDL_GetBasePath()};
	if (cExedir == nullptr) {
		throw ExeDirInitError{};
	}
	std::filesystem::path exedir{cExedir.get()};
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	return exedir;
}

std::filesystem::path tr::getUserDir(const char* org, const char* app)
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	std::unique_ptr<char[], decltype([](void* ptr) { SDL_free(ptr); })> cUserdir{SDL_GetPrefPath(org, app)};
	if (cUserdir == nullptr) {
		throw UserDirInitError{};
	}
	std::filesystem::path userdir{cUserdir.get()};
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	return userdir;
}
