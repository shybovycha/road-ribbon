#ifndef PLAINROADBUILDER_H
#define PLAINROADBUILDER_H

#include <QVector>
#include <QDebug>

#include "vector2d.h"

class PlainRoadBuilder
{
protected:
    Vector2d latlng2xy(Vector2d latlng, Vector2d origin = Vector2d(0, 0));
    QVector<Vector2d> getSidePoints(Vector2d center1, Vector2d center2);
    Vector2d getCrossPoint(Vector2d p1, Vector2d p2, Vector2d p3, Vector2d p4);

public:
    PlainRoadBuilder(QVector<Vector2d> centers, float roadWidth = DEFAULT_ROAD_WIDTH, int mode = SINGLE_POLYGON);

    void build(int mode);

    QVector<Vector2d> points;
    QVector<Vector2d> centers;

    float roadWidth;

    const static float ZOOM = 7.0;
    const static float DEFAULT_ROAD_WIDTH = 15.0;

    enum RenderMode { POLYGON_SET, SINGLE_POLYGON, LINES_ONLY, CENTERS_ONLY };
};

#endif // PLAINROADBUILDER_H
