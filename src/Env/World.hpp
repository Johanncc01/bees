#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include <Interface/Drawable.hpp>
#include <vector>




// Contenu des cellules
enum class Kind : short { herbe, eau, roche };


// Modélisation d'une "graine" permettant la génération du terrain
struct Seed{
    sf::Vector2i coords;
    Kind type;
};

// typedef des types complexes
typedef std::vector<Kind> Grille;
typedef std::vector<sf::Vertex> Vertexes;
typedef std::vector<Seed> Seeds;
typedef std::vector<double> Humids;


class World : public Drawable
{
public :

//Constructeurs
    //Constructeur :
    World();

    //Supprime les copies pour éviter les erreurs
    World(World const&) = delete;
    World& operator=(World const&) = delete;



// Getters pour la taille du monde, ainsi que les id du vector
    float getSize() const;
    int get_id(int, int) const;
    int get_x(int) const;
    int get_y(int) const;
    Vec2d coords_from_pos(Vec2d const&) const;
    double get_humidity(Vec2d const&) const;

// Fonctions graphiques
    // reloadConfig initialise les caractéristiques principales du mondes à partir du fichier JSON utilisé
    void reloadConfig();
    // reloadCacheStructure initalise tous les "Vertexes" de la grille et crée la grille utilisée dans drawOn
    void reloadCacheStructure();
    // updateCache met à jour le renderingCache en fonction du contenu de cells_, puis l'affiche.
    void updateCache();
    // reset appelle les fonctions codées jusqu'ici, et génère un monde vide avec des seeds aléatoires.
    // Le bool en argument permet de définir si la méthode appelle steps et smooths, afin de générer un monde entier aléatoire.
    void reset(bool = true);

    // drawOn se contente de dessiner la texture qui lui est fournie en argument
    void drawOn(sf::RenderTarget&) const;

// Génération aléatoire
    // step fait avancer d'une cellule chaque graine sur la carte dans une direction aléatoire.
    void step();
    // steps appelle step un nombre de fois défini en argument.
    // Le bool détermine si l'affichage doit être rafraîchi après les appels (via updateCache)
    void steps(int, bool = false);

    // smooth calcule pour chaque cellule (d'herbe et de roche) si elle doit être convertie en focntion des cellules voisines
    void smooth();
    // smooths appelle smooth un nombre de fois défini en argument.
    // Le bool détermine si l'affichage doit être rafraîchi après les appels (via updateCache)
    void smooths(int, bool = false);

// Sauvegarde
    // Charge un monde depuis un fichier, en initialisant tous les attributs nécessaires, puis l'affiche avec updateCache.
    void loadFromFile();
    // Sauvegarde le monde actuel dans un fichier, selon les conventions de loadFromFile.
    void saveToFile();

//humidity
    void humidityImpact(size_t);


    bool isGrowable(Vec2d const&) const;

protected:
    int nb_cells;
    float cell_size;
    Grille cells_;

private:

    Vertexes grassVertexes_;
    Vertexes waterVertexes_;
    Vertexes rockVertexes_;
    Vertexes humidityVertexes_;

    sf::RenderTexture renderingCache_;

    Seeds seeds_;
    size_t nb_wseed;
    size_t nb_gseed;


    Humids humidity_lvls;
    double humidityRange;


    // Fonctions d'implémentation
    sf::Vector2i randomN() const;
    void clamp(sf::Vector2i&) const;
};



#endif // WORLD_HPP
