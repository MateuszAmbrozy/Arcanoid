#pragma once

#include <random>

namespace gameRandom
{
inline std::mt19937& engine()
{
    static std::mt19937 generator(std::random_device{}());
    return generator;
}
inline float launchDirection()
{
    return std::uniform_real_distribution<float>(-0.8f, 0.8f)(engine());
}
inline unsigned short power()
{
    // Negative powers are less frequent while every power remains obtainable.
    return static_cast<unsigned short>(std::discrete_distribution<int>({2, 1, 2, 2, 2, 1, 1})(engine()));
}
} // namespace gameRandom
