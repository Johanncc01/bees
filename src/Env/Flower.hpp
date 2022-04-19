#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Collider.hpp>


class Flower : public Collider , public Drawable , public Updatable
{
public:
// Constructeur

    /*!
     * @brief Constructeur de base
     *
     * @param centre de la fleur
     * @param rayon de la fleur
     * @param valeur de pollen initiale
     */
    Flower(Vec2d const&, double, double);

// Méthodes pures

    /*!
     * @brief Dessine la fleur selon ses caractéristiques sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle la fleur est dessinée
     */
    void drawOn(sf::RenderTarget&) const;

    /*!
     * @brief Actualise la fleur et ses caractéristiques sur un temps donné
     *
     * @param temps dt sur lequel il faut actualiser la fleur
     */
    void update(sf::Time);

// Gestion du pollen

    /*!
     * @brief Retire de la fleur la quantité de pollen demandée
     *
     * @param quantité demandée
     *
     * @return quantité qui a pu être retirée
     */
    double takePollen(double);

    /*!
     * @brief Vérifie si un fleur a du pollen
     *
     * @return vrai si la fleur a du pollen
     */
    bool hasPollen() const;

private:

    double pollen;
    sf::Texture texture;
};
