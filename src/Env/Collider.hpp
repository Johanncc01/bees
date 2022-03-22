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
    Collider(Vec2d cen, double rad);
    Collider(Collider const& other);

    // Getters
    const Vec2d& getPosition() const;
    double getRadius() const;

    // Gestion des déplacements
    Vec2d directionTo(Vec2d to) const;
    Vec2d directionTo(Collider const& col) const;
    double distanceTo(Vec2d to) const;
    double distanceTo(Collider const& col) const;
    void move(Vec2d dx);

    // Gestion des collisions
    bool isColliderInside(Collider const& other) const;
    bool isColliding(Collider const& other) const;
    bool isPointInside(Vec2d point) const;

    // Surcharge des opérateurs
    Collider& operator=(Collider source);
    Collider& operator+=(Vec2d dx);
    bool operator>(Collider const& source) const;
    bool operator>(Vec2d source) const;
    bool operator|(Collider const& source) const;

private:
    Vec2d center;
    double radius;
    void clamp();   // Transpose les coordonnées dans le monde torique
};

std::ostream& operator<<(std::ostream&, Collider const& source);
