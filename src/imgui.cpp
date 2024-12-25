#include "../include/tr/dependencies/imgui_impl_opengl3.h"
#include "../include/tr/dependencies/imgui_impl_sdl2.h"
#include "../include/tr/imgui.hpp"
#include "../include/tr/window.hpp"
#include <GL/glew.h>

std::uint64_t tr::ImGui::getTextureID(const Texture& texture) noexcept
{
	return texture._id.get();
}

void tr::ImGui::setTextureFilter(const Texture& texture, MinFilter minFilter, MagFilter magFilter) noexcept
{
	auto id{getTextureID(texture)};
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GLenum(minFilter));
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GLenum(magFilter));
}

void tr::ImGui::initialize()
{
	ImGui_ImplSDL2_InitForOpenGL(tr::window()._impl.get(), tr::window()._glContext._impl.get());
	ImGui_ImplOpenGL3_Init("#version 150");
}

void tr::ImGui::newFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
}

void tr::ImGui::processEvent(const Event& event)
{
	ImGui_ImplSDL2_ProcessEvent((const SDL_Event*)(&event));
}

void tr::ImGui::draw()
{
	ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
}

void tr::ImGui::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
}