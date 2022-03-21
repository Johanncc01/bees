/*
 * POOSV EPFL 2022
 * @authors:
 * Marco Antognini
 */
#include <Application.hpp>
#include <Env/Collider.hpp>
#include <cmath> // abs and fmod
#include <algorithm>

/* TO BE COMPLETED */

Collider::Collider(Vec2d cen, double rad)
    : center(cen)
    , radius(rad)

{
    clamp();
}

Collider::Collider(Collider const& other)
    : center(other.center)
    , radius(other.radius)
{}

const Vec2d& Collider::getPosition() const{
    return center;
}

double Collider::getRadius() const{
    return radius;
}

Collider& Collider::operator=(Collider source){                 // Regarder demain en prog
    std::swap(*this, source);
    return *this;
}

void Collider::clamp(){
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x(); // largeur
    auto height = worldSize.y(); // hauteur

    double reste_x(fmod(center.x(), width));
    if (reste_x < 0){
        reste_x += width;
    }

    double reste_y(fmod(center.y(), height));
    if (reste_y < 0){
        reste_y += height;
    }

    Vec2d newcen(reste_x, reste_y);
    center = newcen;
}


