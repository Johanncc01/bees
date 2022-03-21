/*
 * POOSV EPFL 2022
 * @authors: 
 */

#pragma once
#include <Vec2d>
#include <Utility/Vec2d.hpp>
#include <ostream>

/* TO BE COMPLETED */

class Collider
{
public :
    Collider(const Vec2d& cen, const double& rad)
    : center(cen) , radius(rad)
    {



    }

private :
    Vec2d center;
    double radius;

};

