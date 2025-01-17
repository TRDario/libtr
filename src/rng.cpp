#include "../include/tr/rng.hpp"
#include <ctime>
#include <random>

std::uint64_t tr::generateRandomSeed() noexcept
{
	std::random_device rng;
	return rng() ^ std::time(nullptr);
}

tr::Xorshiftr128p::Xorshiftr128p() noexcept
	: Xorshiftr128p{generateRandomSeed()}
{
}