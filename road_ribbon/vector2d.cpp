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

double Vector2d::distance(Vector2d other)
{
    return sqrt(pow(other.x - this->x, 2.0) + pow(other.y - this->y, 2.0));
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

Vector2d Vector2d::normalize()
{
    double l = this->distance(Vector2d(0, 0));

    return Vector2d(this->x / l, this->y / l);
}
