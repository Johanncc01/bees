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

const Vec2d& Collider::getPosition() const{             // Getter de la position
    return center;
}

double Collider::getRadius() const{                     // Getter du rayon
    return radius;
}

Collider& Collider::operator=(Collider const& source){  // Surcharge de l'opérateur de copie
    center = source.center;
    radius = source.radius;
    return *this;                                       // Retourne une référence pour que l'opération ait une valeur
}

void Collider::clamp(){
    auto worldSize = getApp().getEnvSize();             // Récupération des dimensions prédéfinies
    auto width  = worldSize.x();
    auto height = worldSize.y();

    double reste_x(fmod(center.x(), width));            // Reste de la division afin de contrôler si toujours dans le monde torique
    double reste_y(fmod(center.y(), height));

    if (reste_x < 0){
        reste_x += width;                               // Recalage du x dans les limites si besoin
    }
    if (reste_y < 0){
        reste_y += height;                              // Recalage du y dans les limites si besoin
    }

    Vec2d newcen(reste_x, reste_y);
    center = newcen;                                    // Affectation du centre redimensionné à l'objet
}

Vec2d Collider::directionTo(Vec2d const& to) const{
    Vec2d from(center);
    Vec2d min(to);
    auto worldSize = getApp().getEnvSize();             // Récupération des dimensions prédéfinies
    auto width  = worldSize.x();
    auto height = worldSize.y();

    Vec2d h(0,height);                                  // Création de vecteurs de base à ajouter/soustraire
    Vec2d w(width, 0);                                  // pour trouver le meilleur vecteur "to"

    for (int i(-1); i<2; ++i){                          // La boucle parcourt toutes les possibilités
       for(int j(-1); j<2; ++j){                        // et retient celle avec la plus faible distance
           double dist_min(distance(from, min));
           Vec2d comp(to+i*w+j*h);                      // Toutes les combinaisons (-1, 0, +1) de h et w sont calculées ici
           if (distance(from, comp) < dist_min){
               min = comp;
           }
       }
    }
    return min-from;                                    // Création du vecteur "directionTo" à partir de "from" et du meilleur "to"
}

Vec2d Collider::directionTo(Collider const& col) const{ // Même travail que version précédente mais avec un "Collider" en argument
    Vec2d to(col.getPosition());
    return directionTo(to);
}

double Collider::distanceTo(Vec2d const& to) const{     // Retourne la longueur du vecteur torique calculé par "directionTo"
    return directionTo(to).length();
}

double Collider::distanceTo(Collider const& col) const{ // Surcharge de la méthode précédente mais avec un "Vec2d" en argument"
    Vec2d to(col.getPosition());
    return distanceTo(to);
}

void Collider::move(Vec2d const& dx){                   // ajoute à la position de l'instance courante un "Vec2d"
    center += dx;
    clamp();
}

Collider& Collider::operator+=(Vec2d const& dx){        // Surcharge l'opérateur '+=' lui donnant la même fonction que "move"
    move(dx);
    return *this;
}

bool Collider::isColliderInside(Collider const& other) const{   // Cherche si un "Collider" est à l'intérieur d'un autre "Collider"
    return ((radius >= other.radius) and (distanceTo(other) <= radius - (other.radius)));
}

bool Collider::isColliding(Collider const& other) const{        // Retourne si un "Collider" est en contact avec un autre "Collider"
    return ((distanceTo(other)) <= (radius + other.radius));
}

bool Collider::isPointInside(Vec2d const& point) const{         // Retourne si un point est dans un "Collider"
    return (distanceTo(point) <= radius);
}

bool Collider::operator>(Collider const& source) const{         // Surcharge l'opérateur '>' pour la fonction "isColliderInside" si l'argument est un "Collider"
    return isColliderInside(source);
}

bool Collider::operator>(Vec2d const& source) const{            // Surcharge encore une fois l'opérateur '>' pour la fonction "isPointInside" si l'argument est un "Vec2d"
    return isPointInside(source);
}

bool Collider::operator|(Collider const& source) const{         // Surcharge l'opérateur '|' pour la fonction "isColliding" si l'argument est un "Collider"
    return isColliding(source);
}

std::ostream& operator<<(std::ostream& out, Collider const& source){    // Surcharge l'opérateur '<<' pour formater la sortie d'une instance de la classe "Collider"
    Vec2d center(source.getPosition());
    out << "Collider: position = (" << center.x() << "," << center.y() << ") radius = " << source.getRadius() << std::endl;
    return out;
}





