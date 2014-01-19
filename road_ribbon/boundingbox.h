#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "vector2d.h"

class BoundingBox
{
protected:
    Vector2d minimumBound;
    Vector2d maximumBound;

public:
    BoundingBox();

    void extend(Vector2d point);

    Vector2d getMinimumBound();
    Vector2d getMaximumBound();

    Vector2d getSize();
};

#endif // BOUNDINGBOX_H
