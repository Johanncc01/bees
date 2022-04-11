/*
 * POOSV EPFL 2022
 * @authors: 
 */

#pragma once
#include <Utility/Vec2d.hpp>
#include <ostream>

/* TO BE COMPLETED */

class Collider
{
public:
// Constructeurs

    // Constructeur de base
    Collider(Vec2d const&, double);

    // Constructeur de copie de collider
    Collider(Collider const&);

// Getters

    // Getter de la postion
    const Vec2d& getPosition() const;

    // Getter du rayon
    double getRadius() const;

// Gestion des déplacements

    // Retourne le vecteur en direction du vecteur en argument (depuis l'instance courrante)
    Vec2d directionTo(Vec2d const&) const;
    // Retourne le vecteur en direction du collider en argument
    Vec2d directionTo(Collider const&) const;
    // Retourne la longueur du vecteur torique calculé par "directionTo" jusqu'au vecteur en argument
    double distanceTo(Vec2d const&) const;
    // Retourne la longueur du vecteur torique calculé par "directionTo" jusqu'au collider en argument
    double distanceTo(Collider const&) const;

    // Ajoute à la position de l'instance courante un "Vec2d"
    void move(Vec2d const&);

// Gestion des collisions

    // Retourne si le "Collider" est à l'intérieur d'un autre "Collider" (à l'aide de directionTo)
    bool isColliderInside(Collider const&) const;
    // Retourne si le "Collider" est en contact avec un autre "Collider"
    bool isColliding(Collider const&) const;
    // Retourne si un point est dans le "Collider"
    bool isPointInside(Vec2d const&) const;

// Surcharge des opérateurs

    // Surcharge de l'opérateur d'affectation
    Collider& operator=(Collider const&);
    // Surcharge l'opérateur '+=' lui donnant la même fonction que "move"
    Collider& operator+=(Vec2d const&);
    // Surcharge l'opérateur '>' pour la fonction "isColliderInside" si l'argument est un "Collider"
    bool operator>(Collider const&) const;
    // Surcharge encore une fois l'opérateur '>' pour la fonction "isPointInside" si l'argument est un "Vec2d"
    bool operator>(Vec2d const&) const;
    // Surcharge l'opérateur '|' pour la fonction "isColliding" si l'argument est un "Collider"
    bool operator|(Collider const&) const;


private:
    // Transpose les coordonnées dans le monde torique (fonction d'implémentation privée)
    void clamp();

protected:
    Vec2d center;
    double radius;
};

// Surcharge l'opérateur d'affichage pour formater la sortie d'une instance de la classe "Collider"
std::ostream& operator<<(std::ostream&, Collider const&);
