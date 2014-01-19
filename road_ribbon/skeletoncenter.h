#ifndef SKELETONCENTER_H
#define SKELETONCENTER_H

#include "vector2d.h"
#include "vector3d.h"

class SkeletonCenter
{
public:
    SkeletonCenter();
    SkeletonCenter(Vector2d _location, Vector3d _position, Vector3d _angles);

    bool operator==(const SkeletonCenter &that);

    Vector2d planePos();

    Vector3d position;
    Vector3d angles;
    Vector2d location;
};

#endif // SKELETONCENTER_H
