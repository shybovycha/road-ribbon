#include "skeletoncenter.h"

SkeletonCenter::SkeletonCenter()
{
    this->position = Vector3d(0, 0, 0);
    this->angles = Vector3d(0, 0, 0);
}

SkeletonCenter::SkeletonCenter(Vector2d _location, Vector3d _position, Vector3d _angles)
{
    this->position = _position;
    this->angles = _angles;
    this->location = _location;
}

bool SkeletonCenter::operator==(const SkeletonCenter &that)
{
    return (this->position == that.position) && (this->angles == that.angles);
}

Vector2d SkeletonCenter::planePos()
{
    return Vector2d(this->position.x, this->position.z);
}
