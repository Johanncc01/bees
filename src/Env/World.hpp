#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP
#include <vector>
#include <SFML/Graphics.hpp>


enum class Kind : short { herbe, eau, roche };

struct Seed{
    sf::Vector2i coords;
    Kind type;
};

class World
{
public :
    float getSize() const;

    void reloadConfig();
    void drawOn(sf::RenderTarget&);

    void reloadCacheStructure();
    void updateCache();
    void reset(bool = true);

    void step();
    void steps(int, bool = false);
    void smooth();
    void smooths(int, bool = false);

    int get_id(int, int);
    int get_x(int);
    int get_y(int);

    sf::Vector2i randomN();
    void clamp(sf::Vector2i&);

    void loadFromFile();

private :
    int nb_cells;
    float cell_size;
    std::vector<Kind> cells_;
    std::vector<sf::Vertex> grassVertexes_;
    std::vector<sf::Vertex> waterVertexes_;
    std::vector<sf::Vertex> rockVertexes_;
    sf::RenderTexture renderingCache_;
    std::vector<Seed> seeds_;
    size_t nb_wseed;
    size_t nb_gseed;
};



#endif // WORLD_HPP
