#include "Game.h"
#include "stdafx.h"

namespace
{
void locateAssets(const char* executable)
{
    const auto directory = std::filesystem::absolute(executable).parent_path();
    const std::array<std::filesystem::path, 4> candidates = {directory, directory / "Arcanoid",
                                                             std::filesystem::current_path(),
                                                             std::filesystem::current_path() / "Arcanoid"};
    for (const auto& candidate : candidates)
    {
        if (std::filesystem::is_directory(candidate / "Resources") &&
            std::filesystem::is_directory(candidate / "Config"))
        {
            std::filesystem::current_path(candidate);
            return;
        }
    }
    throw std::runtime_error("Cannot locate Resources and Config next to the executable");
}
} // namespace

int main(int argc, char* argv[])
{
    try
    {
        if (argc > 0)
            locateAssets(argv[0]);
        Game game;
        game.run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Arcanoid: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
