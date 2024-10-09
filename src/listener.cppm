module;
#include <cassert>
#include <AL/al.h>

export module tr:listener;

import std;
import glm;

export namespace tr {
	// Listener orientation struct.
	struct Orientation { glm::vec3 view, up; };
	// View class over the audio context's listener.
    class Listener {
    public:
		// Gets the master gain.
		float gain() const noexcept;
		// Sets the master gain.
		void setGain(float gain) noexcept;

		// Gets the position of the listener.
		glm::vec3 position() const noexcept;
		// Sets the position of the listener.
		void setPosition(const glm::vec3& position) noexcept;

		// Gets the velocity of the listener.
		glm::vec3 velocity() const noexcept;
		// Sets the velocity of the listener.
		void setVelocity(const glm::vec3& velocity) noexcept;

		// Gets the orientation of the listener.
		Orientation orientation() const noexcept;
		// Sets the orientation of the listener.
		void setOrientation(Orientation orientation) noexcept;
	private:
		Listener() noexcept = default;

		friend class AudioContext;
	};
}

// IMPLEMENTATION

float tr::Listener::gain() const noexcept
{
    float gain;
    alGetListenerf(AL_GAIN, &gain);
    return gain;
}

void tr::Listener::setGain(float gain) noexcept
{
    assert(gain >= 0.0f);
    alListenerf(AL_GAIN, gain);
}

glm::vec3 tr::Listener::position() const noexcept
{
    glm::vec3 position;
    alGetListenerfv(AL_POSITION, value_ptr(position));
    return position;
}

void tr::Listener::setPosition(const glm::vec3& position) noexcept
{
    alListenerfv(AL_POSITION, value_ptr(position));
}

glm::vec3 tr::Listener::velocity() const noexcept
{
    glm::vec3 velocity;
    alGetListenerfv(AL_VELOCITY, value_ptr(velocity));
    return velocity;
}

void tr::Listener::setVelocity(const glm::vec3& velocity) noexcept
{
    alListenerfv(AL_VELOCITY, value_ptr(velocity));
}

tr::Orientation tr::Listener::orientation() const noexcept
{
    float arr[6];
    alGetListenerfv(AL_ORIENTATION, arr);
    return { { arr[0], arr[1], arr[2] }, { arr[3], arr[4], arr[5] }};
}

void tr::Listener::setOrientation(Orientation orientation) noexcept
{
    float arr[] { orientation.view.x, orientation.view.y, orientation.view.z, orientation.up.x, orientation.up.y, orientation.up.z };
    alListenerfv(AL_ORIENTATION, arr);
}