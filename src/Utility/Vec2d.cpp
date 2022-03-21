/*
 * POOSV EPFL 2015-2022
 * Marco Antognini
 */

#include <Random/Random.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>

#include <cassert>
#include <cmath>

Vec2d Vec2d::fromAngle(double rad)
{
    return { std::cos(rad), std::sin(rad) };
}

Vec2d Vec2d::fromRandomAngle(double min, double max)
{
    return fromAngle(uniform(min, max));
}

Vec2d Vec2d::fromJSON(j::Value json)
{
    return { json["x"].toDouble(), json["y"].toDouble() };
}

Vec2d::Vec2d()
: Vec2d(0.0, 0.0)
{
    // Done.
}

Vec2d::Vec2d(double x_, double y_)
: x_(x_)
, y_(y_)
{
    // Done.
}

Vec2d::Vec2d(sf::Vector2f const& sfvect)
: Vec2d(sfvect.x, sfvect.y)
{
    // Done.
}

Vec2d::Vec2d(sf::Vector2i const& sfvect)
: Vec2d(sfvect.x, sfvect.y)
{
    // Done.
}

Vec2d::operator sf::Vector2f() const
{
    return { static_cast<float>(x_), static_cast<float>(y_) };
}

Vec2d::operator sf::Vector2i() const
{
    return { static_cast<int>(x_), static_cast<int>(y_) };
}

j::Value Vec2d::save() const
{
    auto vector = j::object();

    vector.set("x", j::number(x_));
    vector.set("y", j::number(y_));

    return vector;
}

Vec2d Vec2d::operator-() const
{
    return { -x_, -y_ };
}

Vec2d Vec2d::operator-(Vec2d const& b) const
{
    return *this + (-b);
}

Vec2d Vec2d::operator+(Vec2d const& b) const
{
    Vec2d ret = *this;
    return ret += b;
}

Vec2d Vec2d::operator*(double c) const
{
    Vec2d ret = *this;
    return ret *= c;
}

Vec2d operator*(double c, Vec2d const& a)
{
    return a * c;
}

Vec2d Vec2d::operator/(double c) const
{
    return *this * (1.0 / c);
}

Vec2d& Vec2d::operator-=(Vec2d const& b)
{
    return *this += (-b);
}

Vec2d& Vec2d::operator+=(Vec2d const& b)
{
    x_ += b.x_;
    y_ += b.y_;
    return *this;
}

Vec2d& Vec2d::operator*=(double c)
{
    x_ *= c;
    y_ *= c;
    return *this;
}

Vec2d& Vec2d::operator/=(double c)
{
    return *this *= (1.0 / c);
}

bool Vec2d::operator==(Vec2d const& b) const
{
    return isEqual(x_, b.x_) && isEqual(y_, b.y_);
}

bool Vec2d::operator!=(Vec2d const& b) const
{
    return !(*this == b);
}

// double& Vec2d::operator[](int axis)
// {
//     switch (axis) {
//     case 0:
//         return x;

//     case 1:
//         return y;

//     default:
//         // This should not happen
//         assert(false);
//         break;
//     }
// }

double Vec2d::operator[](int axis) const
{
    switch (axis) {
    case 0:
        return x_;

    case 1:
        return y_;

    default:
        // This should not happen
        assert(false);
		return 0;
        break;
    }
}

double Vec2d::x() const
{
	return x_;
}

double Vec2d::y() const
{
	return y_;
}

double Vec2d::lengthSquared() const
{
    return x_ * x_ + y_ * y_;
}

double Vec2d::length() const
{
    return std::sqrt(lengthSquared());
}

Vec2d Vec2d::normalised() const
{
    double const len = length();
    return !isEqual(len, 0.0) ? *this / len : *this;
}

Vec2d Vec2d::normal() const
{
    return { y_, -x_ };
}

double distance(Vec2d const& x, Vec2d const& y)
{
    return (x - y).length();
}

Vec2d normal(Vec2d const& a, Vec2d const& b)
{
    return (a - b).normal();
}

double Vec2d::angle() const
{
    return std::atan2(y_, x_);
}

double Vec2d::dot(Vec2d const& other) const
{
    return x_ * other.x_ + y_ * other.y_;
}

int Vec2d::sign(Vec2d const& other) const
{
    if (isEqual(other.lengthSquared(), 0.0) || *this == other) {
        return 0;
    } else {
        return y_ * other.x_ > x_ * other.y_ ? -1 : 1;
    }
}

std::ostream& operator<<(std::ostream& out, Vec2d const& v)
{
    return out << "(" << v.x() << ", " << v.y() << ")";
}

void Vec2d::rotate(double rot)
{
    auto norm = length();
    auto alpha = angle() + rot;
    *this = fromAngle(alpha) * norm;
}
