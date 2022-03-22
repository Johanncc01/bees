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

Collider::Collider(Vec2d const& cen, double rad)        // Constructeur de base
    : center(cen), radius(rad)
{ clamp(); }

Collider::Collider(Collider const& other)               // Constructeur de copie
    : center(other.center), radius(other.radius)
{}                                                      // Aucun appel à clamp() nécessaire car déjà fait dans le Collider copié

const Vec2d& Collider::getPosition() const{
    return center;
}

double Collider::getRadius() const{
    return radius;
}

Collider& Collider::operator=(Collider const& source){
    center = source.center;
    radius = source.radius;
    return *this;
}

void Collider::clamp(){
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x(); // largeur
    auto height = worldSize.y(); // hauteur

    double reste_x(fmod(center.x(), width));
    double reste_y(fmod(center.y(), height));

    if (reste_x < 0){
        reste_x += width;
    }
    if (reste_y < 0){
        reste_y += height;
    }

    Vec2d newcen(reste_x, reste_y);
    center = newcen;
}

Vec2d Collider::directionTo(Vec2d const& to) const{
    Vec2d from(center);
    Vec2d min(to);
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x(); // largeur
    auto height = worldSize.y(); // hauteur

    Vec2d h(0,height);
    Vec2d w(width, 0);

    for (int i(-1); i<2; ++i){
       for(int j(-1); j<2; ++j){
           double dist_min(distance(from, min));
           Vec2d comp(to+i*w+j*h);
           if (distance(from, comp) < dist_min){
               min = comp;
           }
       }
    }
    return min-from;
}

Vec2d Collider::directionTo(Collider const& col) const{
    Vec2d to(col.getPosition());
    return directionTo(to);
}

double Collider::distanceTo(Vec2d const& to) const{
    return directionTo(to).length();
}

double Collider::distanceTo(Collider const& col) const{
    Vec2d to(col.getPosition());
    return distanceTo(to);
}

void Collider::move(Vec2d const& dx){
    center += dx;
    clamp();
}

Collider& Collider::operator+=(Vec2d const& dx){
    move(dx);
    return *this;
}

bool Collider::isColliderInside(Collider const& other) const{
    return ((radius >= other.radius) and (distanceTo(other) <= radius - (other.radius)));
}

bool Collider::isColliding(Collider const& other) const{
    return ((distanceTo(other)) <= (radius + other.radius));
}

bool Collider::isPointInside(Vec2d const& point) const{
    return (distanceTo(point) <= radius);
}

bool Collider::operator>(Collider const& source) const{
    return isColliderInside(source);
}

bool Collider::operator>(Vec2d const& source) const{
    return isPointInside(source);
}

bool Collider::operator|(Collider const& source) const{
    return isColliding(source);
}

std::ostream& operator<<(std::ostream& out, Collider const& source){
    Vec2d center(source.getPosition());
    out << "Collider: position = (" << center.x() << "," << center.y() << ") radius = " << source.getRadius() << std::endl;
    return out;
}





