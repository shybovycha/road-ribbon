#include "boundingbox.h"

BoundingBox::BoundingBox()
{
}

void BoundingBox::extend(Vector2d point)
{
    if (point.x < minimumBound.x)
        minimumBound.x = point.x;

    if (point.y < minimumBound.y)
        minimumBound.y = point.y;

    if (point.x > maximumBound.x)
        maximumBound.x = point.x;

    if (point.y > maximumBound.y)
        maximumBound.y = point.y;
}

Vector2d BoundingBox::getMinimumBound()
{
    return minimumBound;
}

Vector2d BoundingBox::getMaximumBound()
{
    return maximumBound;
}

Vector2d BoundingBox::getSize()
{
    return maximumBound - minimumBound;
}
