#pragma once
#include <Env/Collider.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>


class Flower : public Collider , public Drawable , public Updatable
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur de base
     *
     * @param centre de la fleur
     * @param rayon de la fleur
     * @param valeur de pollen initiale
     */
    Flower(Vec2d const&, double, double);

    /*!
     * @brief Destructeur d'une fleur (par défaut)
     *        Déclaré virtual en prévision de futures sous-classes
     */
    virtual ~Flower() = default;


// Getter

    /*!
     * @brief Vérifie si un fleur a du pollen
     *
     * @return vrai si la fleur a du pollen
     */
    bool hasPollen() const;


// Méthodes pures

    /*!
     * @brief Dessine la fleur selon ses caractéristiques sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle la fleur est dessinée
     */
    void drawOn(sf::RenderTarget&) const override;

    /*!
     * @brief Actualise la fleur et ses caractéristiques sur un temps donné
     *
     * @param temps dt sur lequel il faut actualiser la fleur
     */
    void update(sf::Time) override;


// Gestion du pollen

    /*!
     * @brief Retire de la fleur la quantité de pollen demandée
     *
     * @param quantité demandée
     *
     * @return quantité qui a pu être retirée
     */
    double takePollen(double);


private:

    double pollen;
    sf::Texture texture;

// Fonction d'implémentation

    /*!
     * @brief Affiche le niveau de pollen de la fleur sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle le texte est affiché
     */
    void advancedDebugText(sf::RenderTarget&) const;
};
