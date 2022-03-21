/*
 * POOSV EPFL 2022
 * Marco Antognini
 */

#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <cmath>
#include <algorithm>
#include <numeric>

std::vector<sf::Vertex> generateVertexes(j::Value const& textures, int nbCell, float cellSize)
{
    auto vertexes = std::vector<sf::Vertex>(nbCell * nbCell * 4); // RVO

    // Load the textures
    auto const& grassTexture = getAppTexture(textures["grass"].toString());
    auto const& rockTexture  = getAppTexture(textures["rock"].toString());
    auto const& waterTexture = getAppTexture(textures["water"].toString());

    // Determine which is the smallest dimension
    auto texWidth  = std::min({ grassTexture.getSize().x, rockTexture.getSize().x, waterTexture.getSize().x });
    auto texHeight = std::min({ grassTexture.getSize().y, rockTexture.getSize().y, waterTexture.getSize().y });

    // Generate the vertexes (caches)
    for (int x = 0; x < nbCell; ++x) {
        for (int y = 0; y < nbCell; ++y) {
            auto pos = std::vector<sf::Vector2f>{
                { (x+0.f) * cellSize, (y+0.f) * cellSize },
                { (x+1.f) * cellSize, (y+0.f) * cellSize },
                { (x+1.f) * cellSize, (y+1.f) * cellSize },
                { (x+0.f) * cellSize, (y+1.f) * cellSize }
            };

            // Wrap texture around
            auto size  = static_cast<int>(std::ceil(cellSize));
            auto color = sf::Color::White; // White means not texture recoloring
            auto tex   = std::vector<sf::Vector2f>{
                { static_cast<float>((x+0) * size % texWidth), static_cast<float>((y+0) * size % texHeight) },
                { static_cast<float>((x+1) * size % texWidth), static_cast<float>((y+0) * size % texHeight) },
                { static_cast<float>((x+1) * size % texWidth), static_cast<float>((y+1) * size % texHeight) },
                { static_cast<float>((x+0) * size % texWidth), static_cast<float>((y+1) * size % texHeight) }
            };

            // Update NW, NE, SE, SW vertexes for the current cell
            auto idx = startIndexForCellVertexes(x, y, nbCell);
            for (auto offset : { 0, 1, 2, 3 }) {
                vertexes[idx + offset] = { pos[offset], color, tex[offset] };
            }
        }
    }

    return vertexes;
}

std::size_t startIndexForCellVertexes(int x, int y, int nbCell)
{
    return 4 * (x + y * nbCell);
}

std::vector<std::size_t> indexesForCellVertexes(int x, int y, int nbCell)
{
    std::vector<std::size_t> indexes(4); // RVO
    std::iota(std::begin(indexes), std::end(indexes), startIndexForCellVertexes(x, y, nbCell));
    return indexes;
}
