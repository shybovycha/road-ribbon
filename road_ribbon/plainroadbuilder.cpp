#include "plainroadbuilder.h"

double sec(double x)
{
    return 1.0 / cos(x);
}

PlainRoadBuilder::PlainRoadBuilder(QVector<Vector2d> _centers, float roadWidth, int mode)
{
    if (_centers.size() < 3)
    {
        qDebug() << QString("Given %1 centers. That's not good - we can't build road that simple.").arg(_centers.size());
        return;
    }

    Vector2d origin = this->latlng2xy(_centers[0]);

    for (int i = 1; i < _centers.size(); i++)
    {
        while ((i < _centers.size() - 1) && (_centers[i + 1] == _centers[i])) i++;

        this->centers.push_back(this->latlng2xy(_centers[i], origin));
    }

    this->roadWidth = roadWidth;

    this->build(mode);

    qDebug() << QString("Built polygon with %1 faces (%2 points)").arg(this->points.size() / 4).arg(this->points.size());
}

void PlainRoadBuilder::build(int mode)
{
    if (mode == POLYGON_SET)
    {
        this->points.resize(this->centers.size() * 8);
    } else if (mode == SINGLE_POLYGON)
    {
        this->points.resize(this->centers.size() * 2);
    }

    for (int i = 2; i < this->centers.size() - 2; i += 2)
    {
        QVector<Vector2d> sidePoints;
        Vector2d A, B, C;
        Vector2d A1L, A1R, B1L, B1R, B2L, B2R, C1L, C1R;

        A = centers[i - 2];
        B = centers[i - 1];
        C = centers[i - 0];

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

        int ix = i - 2;

        if (mode == POLYGON_SET)
        {
            this->points[(ix * 8) + 0] = (A1L);
            this->points[(ix * 8) + 1] = (OL);
            this->points[(ix * 8) + 2] = (OR);
            this->points[(ix * 8) + 3] = (A1R);

            this->points[(ix * 8) + 4] = (OL);
            this->points[(ix * 8) + 5] = (B2L);
            this->points[(ix * 8) + 6] = (B2R);
            this->points[(ix * 8) + 7] = (OR);
        } else if (mode == SINGLE_POLYGON)
        {
            static int median = this->centers.size();

            this->points[(ix * 3) + 0] = A1L;
            this->points[(ix * 3) + 1] = OL;
            this->points[(ix * 3) + 2] = C1L;

            this->points[median + (ix * 3) + 0] = A1R;
            this->points[median + (ix * 3) + 1] = OR;
            this->points[median + (ix * 3) + 2] = C1R;
        } else if (mode == LINES_ONLY)
        {
            this->points.push_back(A1L);
            this->points.push_back(A1R);

            this->points.push_back(OL);
            this->points.push_back(OR);

            this->points.push_back(C1L);
            this->points.push_back(C1R);
        } else if (mode == CENTERS_ONLY)
        {
            this->points.push_back(A);
            this->points.push_back(B);
            this->points.push_back(C);
        }
    }
}

Vector2d PlainRoadBuilder::latlng2xy(Vector2d latlng, Vector2d origin)
{
    double  n = pow(10, ZOOM),
            lon_deg = latlng.y,
            lat_rad = (latlng.x * M_PI) / 180.0,
            xtile = n * ((lon_deg + 180.0) / 360.0),
            ytile = n * (1 - (log(tan(lat_rad) + sec(lat_rad)) / M_PI)) / 2;

    return Vector2d(xtile, ytile) - origin;
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

    ap = ap.normalize();

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

    ap = ap.normalize();

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
    B1 = p2.x - p1.x;
    C1 = (p1.x * p2.y) - (p1.y * p2.x);

    A2 = p4.y - p3.y;
    B2 = p4.x - p3.x;
    C2 = (p3.x * p4.y) - (p3.y * p4.x);

    oy = (C2 - ((A2 / A1) * C1)) / ((-A2 / A1) - B2);
    ox = (C1 - (oy * B1)) / A1;

    return Vector2d(ox, oy);
}
