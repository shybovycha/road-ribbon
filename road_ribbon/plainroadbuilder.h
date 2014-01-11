#ifndef PLAINROADBUILDER_H
#define PLAINROADBUILDER_H

#include <QVector>
#include <QDebug>

#include "vector2d.h"

class PlainRoadBuilder
{
protected:
    Vector2d latlng2xy(Vector2d latlng);
    QVector<Vector2d> getSidePoints(Vector2d center1, Vector2d center2);
    Vector2d getCrossPoint(Vector2d p1, Vector2d p2, Vector2d p3, Vector2d p4);

public:
    PlainRoadBuilder(QVector<Vector2d> centers, float roadWidth = PlainRoadBuilder::DEFAULT_ROAD_WIDTH);

    void build();

    QVector<Vector2d> points;
    QVector<Vector2d> centers;

    float roadWidth;

    const static float DEFAULT_ROAD_WIDTH = 15.0;
    const static float LATITUDE_SCALE = 110574.61087757687 / 100000.0;
    const static float LONGITUDE_SCALE = 111302.61697430261 / 100000.0;
};

#endif // PLAINROADBUILDER_H
