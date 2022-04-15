#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>

class Bee : public Drawable , public Updatable
{
public:
    Bee();
};

