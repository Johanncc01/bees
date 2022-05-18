/*
 * POOSV EPFL 2022
 * @authors:
 * Marco Antognini
 */
#include <Env/Collider.hpp>
#include <Application.hpp>
#include <cmath> // abs and fmod
#include <algorithm>


// Constructeurs

Collider::Collider(Vec2d const& cen, double rad)
    : center(cen), radius(rad)
{ clampCenter(); }

Collider::Collider(Collider const& other)
    : center(other.center), radius(other.radius)
{}                                                      // Aucun appel à clamp() nécessaire car déjà fait dans le Collider copié


// Getters

const Vec2d& Collider::getPosition() const{
    return center;
}

double Collider::getRadius() const{
    return radius;
}


// Gestion des déplacements

Vec2d Collider::directionTo(Vec2d const& to) const{
    Vec2d from(center);
    Vec2d min(to);
    auto worldSize = getApp().getEnvSize();             // Récupération des dimensions prédéfinies
    auto width  = worldSize.x();
    auto height = worldSize.y();

    Vec2d h(0,height);                                  // Création de vecteurs de base à ajouter/soustraire
    Vec2d w(width, 0);                                  // pour trouver le meilleur vecteur "to"

    for (int i(-1); i<2; ++i){                          // La boucle parcourt toutes les possibilités
        for (int j(-1); j<2; ++j){                        // et retient celle avec la plus faible distance
            double dist_min(distance(from, min));
            Vec2d comp(to+i*w+j*h);                      // Toutes les combinaisons (-1, 0, +1) de h et w sont calculées ici
            if (distance(from, comp) < dist_min){
                min = comp;
            }
        }
    }
    return min-from;                                    // Création du vecteur "directionTo" à partir de "from" et du meilleur "to"
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
    clampCenter();
}

// Gestion des collisions

bool Collider::isColliderInside(Collider const& other) const{
    return ((radius >= other.radius) and (distanceTo(other) <= radius - (other.radius)));
}

bool Collider::isColliding(Collider const& other) const{
    return ((distanceTo(other)) <= (radius + other.radius));
}

bool Collider::isPointInside(Vec2d const& point) const{
    return (distanceTo(point) <= radius);
}


// Surcharge des opérateurs

Collider& Collider::operator=(Collider const& other){
    center = other.center;
    radius = other.radius;
    return *this;                                       // Retourne une référence pour que l'opération ait une valeur
}

Collider& Collider::operator+=(Vec2d const& dx){
    move(dx);
    return *this;                                       // Retourne une référence pour que l'opération ait une valeur
}

bool Collider::operator>(Collider const& other) const{
    return isColliderInside(other);
}

bool Collider::operator>(Vec2d const& other) const{
    return isPointInside(other);
}

bool Collider::operator|(Collider const& other) const{
    return isColliding(other);
}

std::ostream& operator<<(std::ostream& out, Collider const& col){
    Vec2d center(col.getPosition());
    out << "Collider: position = (" << center.x() << "," << center.y() << ") radius = " << col.getRadius() << std::endl;
    return out;
}


// Implémentation

void Collider::clampCenter(){
    auto worldSize = getApp().getEnvSize();
    auto width  = worldSize.x();                        // Récupération des dimensions prédéfinies
    auto height = worldSize.y();

    double reste_x(fmod(center.x(), width));            // Reste de la division afin de contrôler si toujours dans le monde torique
    double reste_y(fmod(center.y(), height));

    if (reste_x < 0){
        reste_x += width;                               // Recalage dans les limites si besoin
    }
    if (reste_y < 0){
        reste_y += height;
    }

    center = Vec2d(reste_x, reste_y);
}
