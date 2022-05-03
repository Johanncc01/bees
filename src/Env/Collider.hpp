/*
 * POOSV EPFL 2022
 * @authors: 
 */

#pragma once
#include <Utility/Vec2d.hpp>
#include <ostream>


class Collider
{
public:
// Constructeurs

    /*!
     * @brief Constructeur de base.
     *
     * @param centre du collider
     * @param rayon du collider
     */
    Collider(Vec2d const&, double);

    /*!
     * @brief Constructeur de copie
     *
     * @param Collider à copier
     */
    Collider(Collider const&);

    /*!
     * @brief Destructeur du Collider
     */
    ~Collider();

// Getters

    /*!
     * @brief Getter de la position
     *
     * @return le vec2d associé au centre
     */
    const Vec2d& getPosition() const;

    /*!
     * @brief Getter du rayon
     *
     * @return la taille du rayon
     */
    double getRadius() const;

// Gestion des déplacements

    /*!
     * @brief Calcule la direction jusqu'à un point dans le monde torique depuis l'instance
     *
     * @param Vec2d représentant le point
     *
     * @return le Vec2d le plus court jusqu'au point
     */
    Vec2d directionTo(Vec2d const&) const;

    /*!
     * @brief Calcule la direction jusqu'au Collider dans le monde torique depuis l'instance
     *
     * @param Collider à atteindre
     *
     * @return le Vec2d le plus court jusqu'au Collider
     */
    Vec2d directionTo(Collider const&) const;

    /*!
     * @brief Calcule la longueur jusqu'à un point dans le monde torique depuis l'instance (via directionTo)
     *
     * @param Vec2d représentant le point
     *
     * @return la distance jusqu'au point
     */
    double distanceTo(Vec2d const&) const;

    /*!
     * @brief Calcule la longueur jusqu'à un Collider dans le monde torique depuis l'instance (via directionTo)
     *
     * @param Collider à atteindre
     *
     * @return la distance jusqu'au Collider
     */
    double distanceTo(Collider const&) const;

    /*!
     * @brief Ajoute à la position de l'instance un vecteur
     *
     * @param Vec2d à ajouter
     */
    void move(Vec2d const&);

// Gestion des collisions

    /*!
     * @brief Vérifie si le Collider donné est à l'intérieur de l'instance (avec distanceTo)
     *
     * @param Collider à vériifier
     *
     * @return true si le Collider donné est à l'intérieur
     */
    bool isColliderInside(Collider const&) const;

    /*!
     * @brief Vérifie le contact entre l'instance et un Collider donné
     *
     * @param Collider à vérifier
     *
     * @return true si les Colliders sont en contact
     */
    bool isColliding(Collider const&) const;

    /*!
     * @brief Vérifie si un point donné est à l'intérieur de l'instance
     *
     * @param Vec2d représentant le point
     *
     * @return true si le point est dans le Collider
     */
    bool isPointInside(Vec2d const&) const;

// Surcharge des opérateurs

    /*!
     * @brief Surcharge de l'opérateur d'affectation
     *
     * @param Collider à affecter à l'instance
     *
     * @return une référence à l'instance modifiée
     */
    Collider& operator=(Collider const&);

    /*!
     * @brief Surcharge de l'opérateur "+=" avec la méthode "move"
     *
     * @param Vec2d à ajouter à l'instanace
     *
     * @return une référence à l'instance modifiée
     */
    Collider& operator+=(Vec2d const&);

    /*!
     * @brief Surcharge de l'opérateur '>' avec la méthode "isColliderInside"
     *
     * @param Collider à vériifier
     *
     * @return true si le Collider donné est à l'intérieur
     */
    bool operator>(Collider const&) const;

    /*!
     * @brief Surcharge de l'opérateur '>' avec la méthode "isPointInside"
     *
     * @param Vec2d représentant le point
     *
     * @return true si le point est dans le Collider
     */
    bool operator>(Vec2d const&) const;

    /*!
     * @brief Surcharge de l'opérateur '|' avec la méthode "isColliding"
     *
     * @param Collider à vérifier
     *
     * @return true si les Colliders sont en contact
     */
    bool operator|(Collider const&) const;

protected:
    Vec2d center;
    double radius;

    /*!
     * @brief Transpose les coordonnées dans le monde torique (fonction d'implémentation privée)
     */
    void clampCenter();

private:

};

/*!
 * @brief Surcharge de l'opérateur d'affichage pour formater la sortie d'une instance de "Collider"
 *
 * @param Flot de sorite
 * @param Collider à afficher
 *
 * @return le flot de sortie formatté
 */
std::ostream& operator<<(std::ostream&, Collider const&);
