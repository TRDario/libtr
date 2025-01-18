#pragma once
#include <GL/glew.h>
#include <iostream>

namespace tr {
	inline void validateGLCall(const char* file, int line, const char* function) noexcept
	{
		switch (glGetError()) {
		case GL_INVALID_ENUM:
			std::cerr << file << ":" << line << ": GL_INVALID_ENUM raised in function " << function << ".\n";
			std::abort();
			break;
		case GL_INVALID_VALUE:
			std::cerr << file << ":" << line << ": GL_INVALID_VALUE raised in function " << function << ".\n";
			std::abort();
			break;
		case GL_INVALID_OPERATION:
			std::cerr << file << ":" << line << ": GL_INVALID_OPERATION raised in function " << function << ".\n";
			std::abort();
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << file << ":" << line << ": GL_OUT_OF_MEMORY raised in function " << function << ".\n";
			std::abort();
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << file << ":" << line << ": GL_INVALID_FRAMEBUFFER_OPERATION raised in function " << function
					  << ".\n";
			std::abort();
			break;
		default:
			break;
		}
	}

	inline auto validateReturningGLCall(const char* file, int line, const char* function, auto value) noexcept
	{
		switch (glGetError()) {
		case GL_INVALID_ENUM:
			std::cerr << file << ":" << line << ": GL_INVALID_ENUM raised in function " << function << ".\n";
			std::abort();
			break;
		case GL_INVALID_VALUE:
			std::cerr << file << ":" << line << ": GL_INVALID_VALUE raised in function " << function << ".\n";
			std::abort();
			break;
		case GL_INVALID_OPERATION:
			std::cerr << file << ":" << line << ": GL_INVALID_OPERATION raised in function " << function << ".\n";
			std::abort();
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << file << ":" << line << ": GL_OUT_OF_MEMORY raised in function " << function << ".\n";
			std::abort();
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << file << ":" << line << ": GL_INVALID_FRAMEBUFFER_OPERATION raised in function " << function
					  << ".\n";
			std::abort();
			break;
		default:
			break;
		}
		return value;
	}
} // namespace tr

#ifndef NDEBUG
#define TR_GL_CALL_IMPL(file, line, function, ...)                                                                     \
	do {                                                                                                               \
		function(__VA_ARGS__);                                                                                         \
		tr::validateGLCall(file, line, #function);                                                                     \
	} while (0)
#define TR_RETURNING_GL_CALL_IMPL(file, line, function, ...)                                                           \
	validateReturningGLCall(file, line, #function, function(__VA_ARGS__))
#define TR_GL_CALL(function, ...)           TR_GL_CALL_IMPL(__FILE__, __LINE__, function, __VA_ARGS__)
#define TR_RETURNING_GL_CALL(function, ...) TR_RETURNING_GL_CALL_IMPL(__FILE__, __LINE__, function, __VA_ARGS__)
#else
#define TR_GL_CALL(function, ...)           function(__VA_ARGS__)
#define TR_RETURNING_GL_CALL(function, ...) function(__VA_ARGS__)
#endif