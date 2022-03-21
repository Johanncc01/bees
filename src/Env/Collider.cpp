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
