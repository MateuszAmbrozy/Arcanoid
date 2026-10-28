#pragma once
#include "GameConfig.h"

#include <filesystem>
#include <stdexcept>
#include <string>

namespace levels
{
inline constexpr int count = gameConfig::levelCount;
inline std::string filename(int number)
{
    return std::to_string(number) + ".slmp";
}
inline bool exists(int number)
{
    return number >= 1 && number <= count && std::filesystem::is_regular_file(filename(number));
}
inline int next(int current)
{
    for (int number = current + 1; number <= count; ++number)
        if (exists(number))
            return number;
    return 0;
}
inline int number(const std::string& filename)
{
    const auto stem = std::filesystem::path(filename).stem().string();
    std::size_t consumed = 0;
    const int result = std::stoi(stem, &consumed);
    if (consumed != stem.size() || result < 1 || result > count)
        throw std::runtime_error("Invalid level filename: " + filename);
    return result;
}
} // namespace levels
