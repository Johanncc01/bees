#pragma once
#include <Interface/Updatable.hpp>


class FlowerGenerator : public Updatable
{
public:

    /*!
     * @brief Constructeur de base, initialise le temps du compteur à zéro
     */
    FlowerGenerator();

    /*!
     * @brief Destructeur d'un générateur de fleur (par défaut)
     */
    virtual ~FlowerGenerator() = default;

    /*!
     * @brief Place une fleur aléatoirement si le dt donné est supérieur à un seuil (dans la config)
     *
     * @param temps d'actualisation
     */
    void update(sf::Time dt);

    /*!
     * @brief Réininitialise le compteur à zéro
     */
    void reset();

private:
    sf::Time compteur;
};

