#include "../include/tr/rng.hpp"
#include <random>

std::uint64_t generateRandomSeed() noexcept
{
	std::random_device rng;
	return rng() ^ time(nullptr);
}

tr::Xorshiftr128p::Xorshiftr128p() noexcept
	: Xorshiftr128p{generateRandomSeed()}
{
}