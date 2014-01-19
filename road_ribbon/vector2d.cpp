#include "vector2d.h"

Vector2d::Vector2d()
{
    this->x = 0.0;
    this->y = 0.0;
}

Vector2d::Vector2d(double _x, double _y)
{
    this->x = _x;
    this->y = _y;
}

double Vector2d::distanceTo(Vector2d that)
{
    return sqrt(pow(that.x - this->x, 2.0) + pow(that.y - this->y, 2.0));
}

double Vector2d::length()
{
    return this->distanceTo(Vector2d(0, 0));
}

double Vector2d::angleTo(Vector2d that)
{
    return acos(this->dotProduct(that) / (this->length() * that.length()));
}

Vector2d Vector2d::rotate(double angle)
{
    return Vector2d((this->x * cos(angle)) - (this->y * sin(angle)),
                    (this->x * sin(angle)) + (this->y * cos(angle)));
}

Vector2d Vector2d::perpendicular()
{
    if (this->y != 0.0)
    {
        return Vector2d(1.0, -this->x / this->y);
    } else if (this->x != 0.0)
    {
        return Vector2d(-this->y / this->x, 1.0);
    } else
    {
        return Vector2d(0, 0);
    }
}

double Vector2d::dotProduct(Vector2d that)
{
    return (this->x * that.x) + (this->y * that.y);
}

Vector2d Vector2d::operator*(double k)
{
    return Vector2d(this->x * k, this->y * k);
}

Vector2d Vector2d::operator+(const Vector2d &v)
{
    return Vector2d(this->x + v.x, this->y + v.y);
}

Vector2d Vector2d::operator-(const Vector2d &v)
{
    return Vector2d(this->x - v.x, this->y - v.y);
}

bool Vector2d::operator==(const Vector2d &v)
{
    return ((this->x == v.x) && (this->y == v.y));
}

Vector2d Vector2d::normalize()
{
    double l = this->length();

    return Vector2d(this->x / l, this->y / l);
}

sf::Vector2f Vector2d::toSFML()
{
    return sf::Vector2f(this->x, this->y);
}
