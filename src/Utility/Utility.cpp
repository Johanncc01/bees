/*
 * POOSV EPFL 2015-2022
 * Marco Antognini
 */

#include <Utility/Utility.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

Uid createUid()
{
    static Uid last = 0u;

    assert(last < std::numeric_limits<Uid>::max());

    last += 1u;
    return last;
}

std::string to_nice_string(double real)
{
    std::stringstream ss;
    auto digits = static_cast<int>(std::log10(real) + 2);
    ss << std::setprecision(digits) << real;
    return ss.str();
}

sf::Sprite buildSprite(Vec2d const& position, double size, sf::Texture const& texture)
{
    sf::Sprite sprite(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(position);
    double const maxSide = std::max(texture.getSize().x, texture.getSize().y);
    sprite.setScale(Vec2d(size, size) / maxSide);
    return sprite;
}

sf::Text buildText(std::string const& msg, Vec2d const& position, sf::Font const& font, unsigned int size,
                   sf::Color color)
{
    sf::Text txt(msg, font, size);
    txt.setPosition(position);
#if SFML_VERSION_MAJOR >= 2 && (SFML_VERSION_MINOR > 3 || (SFML_VERSION_MINOR == 3 && SFML_VERSION_PATCH >= 2))
    txt.setFillColor(color);
#else
    txt.setColor(color);
#endif
    txt.setCharacterSize(size);
    auto const bounds = txt.getLocalBounds();
    txt.setOrigin(bounds.width / 2, bounds.height / 2);

    return txt;
}

sf::RectangleShape getTextBox(sf::Text& text) {
    sf::FloatRect fRect(text.getGlobalBounds());
    sf::Vector2f size(fRect.width,fRect.height);
    sf::RectangleShape rect;
    rect.setSize(size);
    rect.setPosition(text.getPosition());
    rect.setFillColor(sf::Color(255,255,255,200));
    return rect;
}

sf::CircleShape buildCircle(Vec2d const& position, double radius, sf::Color color)
{
    sf::CircleShape circle(radius, 100);
    circle.setOrigin(radius, radius);
    circle.setPosition(position);
    circle.setFillColor(color);

    return circle;
}

sf::CircleShape buildAnnulus(Vec2d const& position, double radius, sf::Color color, double thickness)
{
    auto const circleRadius = radius - thickness / 2;
    sf::CircleShape annulus(circleRadius, 100);
    annulus.setPosition(position);
    annulus.setOrigin(circleRadius, circleRadius);
    annulus.setFillColor(sf::Color::Transparent);
    annulus.setOutlineThickness(thickness);
    annulus.setOutlineColor(color);

    return annulus;
}

sf::RectangleShape buildSquare(Vec2d const& position, double side, sf::Color color)
{
    Vec2d const size{ side, side };
    sf::RectangleShape square(size);
    square.setPosition(position);
    square.setOrigin(size / 2.0);
    square.setFillColor(color);

    return square;
}

sf::RectangleShape buildRectangle(Vec2d const& topLeft, Vec2d const& bottomRight,
                                  sf::Color borderColor, double borderThickness,
                                  sf::Color fillColor)
{
    Vec2d size = bottomRight - topLeft;
    sf::RectangleShape rect(size);
    rect.setPosition(topLeft + size / 2.0);
    rect.setOrigin(size / 2.0);
    rect.setFillColor(fillColor);
    rect.setOutlineThickness(borderThickness);
    rect.setOutlineColor(borderColor);

    return rect;
}

sf::RectangleShape buildLine(Vec2d const& start, Vec2d const& end, sf::Color color, double thickness)
{
    auto const length = distance(start, end);
    auto angle = (end - start).angle();

    sf::RectangleShape line({ static_cast<float>(0), static_cast<float>(length) });
    line.setPosition(start);
    line.setOrigin(0, 0);
    line.setRotation(angle / DEG_TO_RAD - 90);
    line.setOutlineThickness(thickness);
    line.setOutlineColor(color);

    return line;
}

bool isEqual(double x, double y)
{
    return isEqual(x, y, EPSILON);
}

bool isEqual(double x, double y, double epsilon)
{
    return std::abs(x - y) < epsilon;
}

double angleDelta(double a, double b)
{
    double delta = a - b;
    while (delta < -PI)
        delta += TAU;
    while (delta >= PI)
        delta -= TAU;
    return delta;
}

std::vector<std::string> split(std::string const& str, char delim)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string currentToken;

    while (std::getline(ss, currentToken, delim)) {
        tokens.push_back(currentToken);
    }

    return tokens;
}
