#pragma once

namespace gameConfig
{
inline constexpr int gridColumns = 32;
inline constexpr int gridRows = 36;
inline constexpr int levelCount = 16;
inline constexpr int initialLives = 3;
inline constexpr unsigned maxBalls = 64;
inline constexpr float inversePaddleDuration = 10.f;
inline constexpr int tileWidth = 100;
inline constexpr int tileHeight = 50;
inline constexpr int atlasColumns = 9;
inline constexpr int atlasRows = 6;
} // namespace gameConfig

namespace physics
{
inline constexpr float pixelsPerMeter = 30.f;
inline constexpr float fixedTimeStep = 1.f / 60.f;
inline constexpr float maxFrameTime = 0.1f;
} // namespace physics
