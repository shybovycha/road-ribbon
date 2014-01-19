#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>
#include <SFML/Graphics.hpp>

class Vector2d
{
public:
    Vector2d();
    Vector2d(double _x, double _y);

    double distanceTo(Vector2d other);
    double angleTo(Vector2d other);
    double dotProduct(Vector2d other);
    double length();

    Vector2d rotate(double angle);
    Vector2d normalize();
    Vector2d perpendicular();

    Vector2d operator*(double k);

    Vector2d operator+(const Vector2d &v);
    Vector2d operator-(const Vector2d &v);

    sf::Vector2f toSFML();

    bool operator==(const Vector2d &v);

    double x;
    double y;
};

#endif // VECTOR2D_H
