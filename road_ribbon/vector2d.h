#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>

class Vector2d
{
public:
    Vector2d();
    Vector2d(double _x, double _y);

    double distance(Vector2d other);

    Vector2d operator*(double k);

    Vector2d operator+(const Vector2d &v);
    Vector2d operator-(const Vector2d &v);

    Vector2d normalize();

    double x;
    double y;
};

#endif // VECTOR2D_H
