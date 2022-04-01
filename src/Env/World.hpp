#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP
#include <vector>
#include <SFML/Graphics.hpp>

enum class Kind : short { herbe, eau, roche };

typedef std::vector<Kind> Grille;
typedef std::vector<sf::Vertex> Vertexes;

struct Seed{
    sf::Vector2i coords;
    Kind type;
};

typedef std::vector<Seed> Seeds;

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
    void saveToFile();

private :
    int nb_cells;
    float cell_size;
    Grille cells_;
    Vertexes grassVertexes_;
    Vertexes waterVertexes_;
    Vertexes rockVertexes_;
    sf::RenderTexture renderingCache_;
    Seeds seeds_;
    size_t nb_wseed;
    size_t nb_gseed;
};



#endif // WORLD_HPP
