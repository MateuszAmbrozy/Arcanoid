#pragma once

#include <stdexcept>
#include <string>

namespace assets
{
template <typename Resource> void load(Resource& resource, const std::string& path)
{
    if (!resource.loadFromFile(path))
        throw std::runtime_error("Cannot load asset: " + path);
}
} // namespace assets
