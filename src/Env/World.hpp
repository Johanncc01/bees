#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>
#include <Utility/Vec2d.hpp>
#include <vector>


// Contenu des cellules
enum class Kind : short { herbe, eau, roche };


// Modélisation d'une "graine" permettant la génération du terrain
struct Seed{
    sf::Vector2i coords;
    Kind type;
};

// typedef des types complexes
typedef std::vector<Kind> Cells;
typedef std::vector<sf::Vertex> Vertexes;
typedef std::vector<Seed> Seeds;
typedef std::vector<double> Humidities;


class World : public Drawable
{
public :

// Constructeurs

    /*!
     * @brief Constructeur par défaut, appelant reset() qui recharge la configuration à partir du .json.
     */
    World();
    //~World();      -> besoin ???

    // Interdit les copies pour éviter les erreurs
    World(World const&) = delete;
    World& operator=(World const&) = delete;

// Getters

    /*!
     * @brief Getter de la taille du monde
     *
     * @return le float valant nb_cells * cell_size
     */
    float getSize() const;

    /*!
     * @brief Getter du niveau d'humidité
     *
     * @param position de la cellule
     *
     * @return niveau d'humidité de la cellule
     */
    double getHumidity(Vec2d const&) const;

    /*!
     * @brief Getter de l'id associé aux coordonnées
     *
     * @param coordonnée x
     * @param coordonnée y
     *
     * @return l'id en fontion de l'instance
     */
    int getId(int, int) const;

    /*!
     * @brief Getter de la coordonnée x
     *
     * @param id de la cellule
     *
     * @return coordonnée x en fonction de l'instance
     */
    int getX(int) const;

    /*!
     * @brief Getter de la coordonnée y
     *
     * @param id de la cellule
     *
     * @return coordonnée y en fonction de l'instance
     */
    int getY(int) const;


// Fonctions graphiques

    /*!
     * @brief Initialise les caractéristiques principales du monde à partir du .json utilisé
     */
    void reloadConfig();

    /*!
     * @brief Initialise tous les "Vertexes" de la grille et crée la grille utilisée dans drawOn
     */
    void reloadCacheStructure();

    /*!
     * @brief Met à jour le renderingCache en fonction du contenu de cells_, puis l'affiche.
     */
    void updateCache();

    /*!
     * @brief Génère un monde vide (à partir du fichier .json) avec des seeds aléatoires.
     *
     * @param si vrai, appelle "steps" et "smooths", pour générer un monde entier aléatoire (vrai par défaut)
     */
    void reset(bool = true);

    /*!
     * @brief Dessine le monde et ses caractéristiques sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle le monde est dessiné
     */
    void drawOn(sf::RenderTarget&) const;

// Génération aléatoire

    /*!
     * @brief Fait avancer d'une cellule chaque graine sur la carte dans une direction aléatoire
     */
    void step();

    /*!
     * @brief Appelle step() un nombre de fois donné
     *
     * @param nombre d'itérations demandé
     * @param si vrai, rafraîchit l'affichage après les appels à step() (via updateCache)
     */
    void steps(int, bool = false);

    /*!
     * @brief Calcule pour chaque cellule (d'herbe et de roche) si elle doit être convertie en focntion des cellules voisines
     */
    void smooth();

    /*!
     * @brief Appelle smooth() un nombre de fois donné
     *
     * @param nombre d'itérations demandé
     * @param si vrai, rafraîchit l'affichage après les appels à smooth() (via updateCache)
     */
    void smooths(int, bool = false);

// Sauvegarde

    /*!
     * @brief Charge un monde depuis un fichier, en initialisant tous les attributs nécessaires, puis l'affiche avec updateCache()
     */
    void loadFromFile();

    /*!
     * @brief Sauvegarde le monde actuel dans un fichier, selon les conventions de loadFromFile()
     */
    void saveToFile();

// Humidity
    /*!
     * @brief Calcule l'impact d'humidité d'une cellule donnée sur le monde, puis l'ajoute au tableau humidity_
     *
     * @param id de la cellule à évaluer
     */
    void humidityImpact(size_t);

// Tests

    /*!
     * @brief Calcule si une position est libre pour y faire pousser une fleur
     *
     * @param position de la cellule à tester
     *
     * @return vrai si la cellule est libre
     */
    bool isGrowable(Vec2d const&) const;

    /*!
     * @brief Calcule si une position est libre pour y placer une ruche
     *
     * @param position de la cellule à tester
     *
     * @return vrai si la cellule est libre
     */
    bool isHiveable(Vec2d const&, double) const;

private:

    int nb_cells;
    float cell_size;
    Cells cells_;

    Vertexes grassVertexes_;
    Vertexes waterVertexes_;
    Vertexes rockVertexes_;
    Vertexes humidityVertexes_;

    sf::RenderTexture renderingCache_;

    Seeds seeds_;

    Humidities humidity_;
    double humidityRange;

    // Fonctions d'implémentation

    /*!
     * @brief Génère un vecteur unitaire dans une direction aléatoire
     *
     * @return le Vector2i généré
     */
    sf::Vector2i randomDir() const;

    /*!
     * @brief Modifie le vecteur donné pour qu'il soit toujours à l'intérieur du monde
     *
     * @param le Vector2i à modifier
     */
    void clamp(sf::Vector2i&) const;

    /*!
     * @brief Transpose un vecteur de position (SFML) donné en coordonnées utilisables dans les tableaux de World
     *
     * @param le Vec2d à transposer
     */
    Vec2d coords_from_pos(Vec2d const&) const;
};

