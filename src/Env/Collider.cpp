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

Vec2d Collider::directionTo(Vec2d to) const{
    Vec2d from(center);
    Vec2d max(to);
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x(); // largeur
    auto height = worldSize.y(); // hauteur

    Vec2d h(0,height);
    Vec2d w(width, 0);

    for (int i(-1); i<2; ++i){
       for(int j(-1); j<2; ++j){
           double dist_max(distance(from, max));
           Vec2d comp(to+i*w+j*h);
           if (distance(from, comp) < dist_max){
               max = comp;
           }
       }
    }
    return max;
}

Vec2d Collider::directionTo(Collider const& col) const{
    Vec2d to(col.getPosition());
    return directionTo(to);
}

double Collider::distanceTo(Vec2d to) const{
    return directionTo(to).length();
}

double Collider::distanceTo(Collider const& col) const{
    Vec2d to(col.getPosition());
    return distanceTo(to);
}

void Collider::move(Vec2d dx){
    center += dx;
    clamp();
}
