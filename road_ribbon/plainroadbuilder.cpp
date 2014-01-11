#include "plainroadbuilder.h"

PlainRoadBuilder::PlainRoadBuilder(QVector<Vector2d> centers, float roadWidth)
{
    if (centers.size() < 3)
    {
        qDebug() << QString("Given %1 centers. That's not good - we can't build road that simple.").arg(centers.size());
        return;
    }

    this->centers = centers;
    this->roadWidth = roadWidth;

    this->build();

    qDebug() << QString("Built polygon with %1 faces (%2 points)").arg(this->points.size() / 4).arg(this->points.size());
}

void PlainRoadBuilder::build()
{
    for (int i = 2; i < this->centers.size(); i++)
    {
        QVector<Vector2d> sidePoints;
        Vector2d A, B, C;
        Vector2d A1L, A1R, B1L, B1R, B2L, B2R, C1L, C1R;

        A = this->latlng2xy(centers[i - 2]);
        B = this->latlng2xy(centers[i - 1]);
        C = this->latlng2xy(centers[i - 0]);

        sidePoints = this->getSidePoints(A, B);
        A1L = sidePoints[0];
        A1R = sidePoints[1];
        B1L = sidePoints[2];
        B1R = sidePoints[3];

        sidePoints = this->getSidePoints(B, C);
        B2L = sidePoints[0];
        B2R = sidePoints[1];
        C1L = sidePoints[2];
        C1R = sidePoints[3];

        Vector2d OL, OR;

        OL = this->getCrossPoint(A1L, B1L, B2L, C1L);
        OR = this->getCrossPoint(A1R, B1R, B2R, C1R);

        this->points.push_back(A1L);
        this->points.push_back(OL);
        this->points.push_back(OR);
        this->points.push_back(A1R);

        this->points.push_back(OL);
        this->points.push_back(B2L);
        this->points.push_back(B2R);
        this->points.push_back(OR);
    }
}

Vector2d PlainRoadBuilder::latlng2xy(Vector2d latlng)
{
    Vector2d xy(latlng.x * PlainRoadBuilder::LATITUDE_SCALE, latlng.y * PlainRoadBuilder::LONGITUDE_SCALE);

    return xy;
}

QVector<Vector2d> PlainRoadBuilder::getSidePoints(Vector2d center1, Vector2d center2)
{
    QVector<Vector2d> results;
    Vector2d a, ap;
    Vector2d AL, AR, BL, BR;

    a = center1 - center2;

    if (a.y != 0.0)
    {
        ap = Vector2d(1.0, -a.x / a.y);
    } else if (a.x != 0.0)
    {
        ap = Vector2d(-a.y / a.x, 1.0);
    }

    AL = center1 + (ap * this->roadWidth);
    AR = center1 - (ap * this->roadWidth);

    a = center2 - center1;

    if (a.y != 0.0)
    {
        ap = Vector2d(1.0, -a.x / a.y);
    } else if (a.x != 0.0)
    {
        ap = Vector2d(-a.y / a.x, 1.0);
    }

    BL = center2 + (ap * this->roadWidth);
    BR = center2 - (ap * this->roadWidth);

    results.push_back(AL);
    results.push_back(AR);
    results.push_back(BL);
    results.push_back(BR);

    return results;
}

Vector2d PlainRoadBuilder::getCrossPoint(Vector2d p1, Vector2d p2, Vector2d p3, Vector2d p4)
{
    double A1, B1, C1, A2, B2, C2, ox, oy;

    A1 = p2.y - p1.y;
    B1 = p1.x - p2.x;
    C1 = (p2.x * p1.y) - (p1.x * p1.y);

    A2 = p4.y - p3.y;
    B2 = p3.x - p4.x;
    C2 = (p4.x * p3.y) - (p3.x * p3.y);

    ox = ((B1 * C2) - (B2 * C1)) / ((A1 * B2) - (A2 * B1));
    oy = ((C1 * A2) - (C2 * A1)) / ((A1 * B2) - (A2 * B1));

    return Vector2d(ox, oy);
}
