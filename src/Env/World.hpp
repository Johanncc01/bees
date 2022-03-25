#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP
#include <vector>
#include <SFML/Graphics.hpp>

enum class Kind : short { herbe, eau, roche };

class World
{
public :
    float getSize() const;

    void reloadConfig();
    void drawOn(sf::RenderTarget& target);

    void reloadCacheStructure();
    void updateCache();
    void reset(bool = true);

private :
    int nb_cells;
    float cell_size;
    std::vector<Kind> cells_;
    std::vector<sf::Vertex> grassVertexes_;
    std::vector<sf::Vertex> waterVertexes_;
    std::vector<sf::Vertex> rockVertexes_;
    sf::RenderTexture renderingCache_;
};

#endif // WORLD_HPP
