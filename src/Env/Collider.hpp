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
    Collider(Vec2d cen, double rad);
    Collider(Collider const& other);

    const Vec2d& getPosition() const;
    double getRadius() const;
    Collider& operator=(Collider source);

    Vec2d directionTo(Vec2d to) const;


private:
    Vec2d center;
    double radius;
    void clamp();
};

