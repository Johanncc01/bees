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
    Collider(Vec2d const&, double);
    Collider(Collider const&);

    // Getters
    const Vec2d& getPosition() const;
    double getRadius() const;

    // Gestion des déplacements
    Vec2d directionTo(Vec2d const&) const;
    Vec2d directionTo(Collider const&) const;
    double distanceTo(Vec2d const&) const;
    double distanceTo(Collider const&) const;
    void move(Vec2d const&);

    // Gestion des collisions
    bool isColliderInside(Collider const&) const;
    bool isColliding(Collider const&) const;
    bool isPointInside(Vec2d const&) const;

    // Surcharge des opérateurs
    Collider& operator=(Collider const&);
    Collider& operator+=(Vec2d const&);
    bool operator>(Collider const&) const;
    bool operator>(Vec2d const&) const;
    bool operator|(Collider const&) const;

private:
    Vec2d center;
    double radius;
    void clamp();   // Transpose les coordonnées dans le monde torique (fonction d'implémentation privée)
};

// Surcharge externe de l'opérateur d'affichage
std::ostream& operator<<(std::ostream&, Collider const&);
