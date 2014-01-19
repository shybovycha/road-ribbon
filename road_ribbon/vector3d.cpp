#include "vector3d.h"

Vector3d::Vector3d()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

Vector3d::Vector3d(double _x, double _y, double _z)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
}

Vector3d::Vector3d(const Vector3d &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

double Vector3d::distanceTo(Vector3d that)
{
    return sqrt(pow(this->x - that.x, 2.0) + pow(this->y - that.y, 2.0) + pow(this->z - that.z, 2.0));
}

bool Vector3d::operator=(const Vector3d &v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

Vector3d Vector3d::operator-(const Vector3d &that)
{
    return Vector3d((this->x - that.x), (this->y - that.y), (this->z - that.z));
}

Vector3d Vector3d::operator+(const Vector3d &that)
{
    return Vector3d((this->x + that.x), (this->y + that.y), (this->z + that.z));
}

Vector3d Vector3d::operator*(double v)
{
    return Vector3d(this->x * v, this->y * v, this->z * v);
}

Vector3d Vector3d::operator/(double v)
{
    return Vector3d(this->x / v, this->y / v, this->z / v);
}

bool Vector3d::operator==(const Vector3d &v)
{
    return (this->distanceTo(v) < pow(10.0, -5.0));
}

Vector3d Vector3d::operator+=(const Vector3d &v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;

    return *this;
}

Vector3d Vector3d::operator-=(const Vector3d &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;

    return *this;
}

Vector3d Vector3d::operator*=(double v)
{
    this->x *= v;
    this->y *= v;
    this->z *= v;

    return *this;
}

Vector3d Vector3d::operator/=(double v)
{
    this->x /= v;
    this->y /= v;
    this->z /= v;

    return *this;
}

double Vector3d::length()
{
    return sqrt(pow(this->x, 2.0) + pow(this->y, 2.0) + pow(this->z, 2.0));
}

double Vector3d::dotProduct(Vector3d that)
{
    return (this->x * that.x) + (this->y * that.y) + (this->z * that.z);
}

double Vector3d::angleTo(Vector3d that)
{
    return acos(this->dotProduct(that) / (this->length() * that.length()));
}

Vector3d Vector3d::normalize()
{
    double l = this->length();
    return Vector3d(this->x / l, this->y / l, this->z / l);
}

Vector3d Vector3d::crossProduct(const Vector3d &that)
{
    return Vector3d((this->y * that.z) - (this->z * that.y),
                    (this->z * that.x) - (this->x * that.z),
                    (this->x * that.y) - (this->y * that.x));
}

Vector3d Vector3d::rotate(Vector3d axis, double radianAngle)
{
    Vector3d v = *this, u = axis;

    return ((v - (u * (u.dotProduct(v)))) * cos(radianAngle)) +
            (u.crossProduct(v) * sin(radianAngle)) +
            (u * (u.dotProduct(v)));
}
