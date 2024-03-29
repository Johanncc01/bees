#pragma once
#include <Interface/Drawable.hpp>
#include <Utility/Vec2d.hpp>

#include <vector>


// Contenu des cellules
enum class Kind : short { herbe, eau, roche };


// Modélisation d'une "graine" permettant la génération du terrain
struct Seed{
    Vec2d coords;
    Kind type;
};

// typedef des types complexes
typedef std::vector<Kind> Cells;
typedef std::vector<sf::Vertex> Vertexes;
typedef std::vector<Seed> Seeds;
typedef std::vector<double> Humidities;
typedef std::vector<std::size_t> Ids;


class World : public Drawable
{
public :
// Constructeurs

    /*!
     * @brief Constructeur par défaut, appelant reset() qui recharge la configuration à partir du .json.
     */
    World();

    /*!
     * @brief Destructeur du terrain (par défaut)
     */
    ~World() = default;

    // Interdit les copies et affectations pour éviter les erreurs
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
    void drawOn(sf::RenderTarget&) const override;

    /*!
     * @brief Transpose les coordonnées données dans le monde torique
     *
     * @param Vec2d à traiter
     *
     * @return Vec2d à l'intérieur du monde torique
     *
     * @note en public car appel nécessaire dans Env
     */
     Vec2d toricClamp(Vec2d const&) const;


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


// Tests (fleurs et ruches)

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
     * @param rayon du collider associé à la ruche
     *
     * @return vrai si la cellule est libre
     */
    bool isHiveable(Vec2d const&, double) const;

    /*!
     * @brief Calcule si une position est libre pour voler
     *
     * @param position de la cellule à tester
     *
     * @return vrai si une abeille est apte à voler sur la cellule
     */
    bool isFlyable(Vec2d const&) const;


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
     * @brief Modifie les tableaux de Vertexes selon cells_ et humidity_, avant l'affichage sur un renderingCache
     *
     * @param x de la cellule à vérifier
     * @param y de la cellule à vérifier
     * @param niveau maximum d'humidité du tableau (pour l'affichage relatif)
     * @param niveau minimum d'humidité du tableau (pour l'affichage relatif)
     */
    void updateVertexes(int, int, double, double);

    /*!
     * @brief Initialise les graines selon la configuration (après un reset())
     */
    void seedsReset();

    /*!
     * @brief Calcule le nombre de cellules d'eau et d'herbes voisines à une cellule d'id donné
     *
     * @param id de la cellule à analyser
     * @param nombre de cellules voisines (modifié par la fonction)
     * @param nombre de cellules d'eau voisines (modifié par la fonction)
     * @param nombre de cellules d'herbe voisines (modifié par la fonction)
     */
    void cellCounter(size_t, double&, double&, double&) const;

    /*!
     * @brief Génère un vecteur unitaire dans une direction aléatoire
     *
     * @return le Vec2d généré
     */
    Vec2d randomDir() const;

    /*!
     * @brief Modifie le vecteur donné pour qu'il soit toujours à l'intérieur du monde
     *
     * @param le Vec2d à modifier
     *
     * @return le Vec2d à l'intérieur du monde
     */
    Vec2d regularClamp(Vec2d const&) const;

    /*!
     * @brief Transpose un vecteur de position (SFML) donné en coordonnées utilisables dans les tableaux de World
     *
     * @param le Vec2d à transposer
     *
     * @return le Vec2d associé aux coordonées du monde
     */
    Vec2d coordsFromPos(Vec2d const&) const;

    /*!
     * @brief Calcule l'ensemble des indices à l'intérieur d'une zone du monde torique
     *
     * @param position du topLeft
     * @param position du bottomRight
     *
     * @return l'ensemble des indices à l'intérieur d'un rectangle dans le monde torique
     */
    Ids indexesForRect(Vec2d const&, Vec2d const&) const;

    /*!
     * @brief Ajoute à un tableau donné les ids correspondants à deux "for loops" paramétrées par des arguments
     *
     * @param les indices de départ et de fin des bouches (x4)
     * @param tableau d'Ids à modifier
     */
    void doubleForLoop(size_t, size_t, size_t, size_t, Ids&) const;

};

