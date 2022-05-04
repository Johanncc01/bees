#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Collider.hpp>


// Prédéclaration de la classe
class Bee;

// typedef des types complexes
typedef std::vector<Bee*> Bees;

class Hive : public Collider, public Drawable, public Updatable
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur de base d'une ruche
     *
     * @param centre de la ruche
     * @param rayon de la ruche
     */
    Hive(Vec2d const&, double);

    /*!
     * @brief Destructeur de la ruche, qui détruit les abeilles
     */
    ~Hive();

    // Interdit les copies et affectations pour éviter les erreurs
    Hive(Hive const&) = delete;
    Hive& operator=(Hive const&) = delete;

// Méthodes pures

    /*!
     * @brief Dessine la ruche selon ses caractéristiques sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle la ruche est dessinée
     */
    void drawOn(sf::RenderTarget&) const;

    /*!
     * @brief Actualise la ruche et ses caractéristiques sur un temps donné
     *
     * @param temps dt sur lequel il faut actualiser la ruche
     */
    void update(sf::Time);

// Pollen

    /*!
     * @brief Dépose une valeur donnée de pollen dans la ruche
     *
     * @param quantité de pollen à déposer
     */
    double dropPollen(double);

    /*!
     * @brief Retire de la ruche la quantité de pollen demandée
     *
     * @param quantité demandée
     *
     * @return quantité qui a pu être retirée
     */
    double takePollen(double);

protected:
// Bees

        /*!
         * @brief Ajoute une abeille dans la ruche
         */
        Bee* addBee(double scoutProb = 0);

private:

    Bees bees;
    double pollen;

};

