#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>

class Vector3d
{
public:
    Vector3d();
    Vector3d(const Vector3d &v);
    Vector3d(double _x, double _y, double _z);

    Vector3d operator-(const Vector3d &v);
    Vector3d operator+(const Vector3d &v);
    Vector3d operator*(double v);
    Vector3d operator/(double v);
    Vector3d operator+=(const Vector3d &v);
    Vector3d operator-=(const Vector3d &v);
    Vector3d operator*=(double v);
    Vector3d operator/=(double v);

    bool operator=(const Vector3d &v);

    bool operator==(const Vector3d &v);

    double distanceTo(Vector3d that);
    double angleTo(Vector3d that);
    double dotProduct(Vector3d that);

    double length();

    Vector3d normalize();
    Vector3d crossProduct(const Vector3d &that);
    Vector3d rotate(Vector3d axis, double radianAngle);

    double x;
    double y;
    double z;
};

#endif // VECTOR3D_H
