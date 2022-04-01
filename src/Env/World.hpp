#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP
#include <vector>
#include <SFML/Graphics.hpp>
//Contenu des cellules
enum class Kind : short { herbe, eau, roche };


//Modelisation d'une "graine" permettant la génération du terrain
struct Seed{
    sf::Vector2i coords;
    Kind type;
};

//typedef des types complexes
typedef std::vector<Kind> Grille;
typedef std::vector<sf::Vertex> Vertexes;
typedef std::vector<Seed> Seeds;

class World
{
public :

// getter pour la taille du monde
    float getSize() const;
    int get_id(int, int);
    int get_x(int);
    int get_y(int);

// Fonctions graphiques pour afficher le monde

    // drawOn se contente de dessiner la texture qui lui est fournie en argument
    void drawOn(sf::RenderTarget&);

    // reloadConfig initialise les caractéristiques principales du mondes à partir du fichier JSON utilisé
    void reloadConfig();
    void reloadCacheStructure();
    void updateCache();
    void reset(bool = true);

// step

    void step();
    void steps(int, bool = false);

// smooth
    //
    void smooth();
    void smooths(int, bool = false);


     sf::Vector2i randomN();
    void clamp(sf::Vector2i&);

//Flot
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
