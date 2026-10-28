#pragma once

#include <filesystem>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace maps
{
struct Tile
{
    int x{}, y{}, column{}, row{}, textureX{}, textureY{}, lives{}, type{};
};

struct Map
{
    int width{}, height{};
    std::string texture;
    std::vector<Tile> tiles;
};

// Parse and validate before changing any live SFML or Box2D objects.
inline Map load(const std::filesystem::path& path)
{
    std::ifstream input(path);
    if (!input)
        throw std::runtime_error("Cannot open map: " + path.string());
    Map map;
    if (!(input >> map.width >> map.height >> map.texture) || map.width < 1 || map.height < 1 ||
        map.width > 128 || map.height > 128)
        throw std::runtime_error("Invalid map header: " + path.string());

    const std::filesystem::path texture(map.texture);
    if (texture.is_absolute() || texture.has_root_name())
        throw std::runtime_error("Map texture must use a relative asset path");
    for (const auto& part : texture)
        if (part == "..")
            throw std::runtime_error("Map texture cannot leave the asset directory");

    std::set<std::pair<int, int>> occupied;
    input >> std::ws;
    while (input.peek() != std::char_traits<char>::eof())
    {
        Tile tile;
        if (!(input >> tile.x >> tile.y >> tile.column >> tile.row >> tile.textureX >> tile.textureY >>
              tile.lives >> tile.type))
            throw std::runtime_error("Incomplete tile record: " + path.string());
        if (tile.x < 0 || tile.x >= map.width || tile.y < 0 || tile.y >= map.height || tile.column < 0 ||
            tile.column >= 9 || tile.row < 0 || tile.row >= 6 || tile.textureX < 0 || tile.textureX >= 900 ||
            tile.textureY < 0 || tile.textureY >= 300 || tile.lives < 0 || tile.lives > 5 || tile.type < 0 ||
            tile.type > 2 || !occupied.emplace(tile.x, tile.y).second)
            throw std::runtime_error("Invalid or duplicate tile: " + path.string());
        map.tiles.push_back(tile);
        input >> std::ws;
    }
    return map;
}
} // namespace maps
