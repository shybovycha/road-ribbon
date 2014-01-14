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

void PlainRoadBuilder::buildPolygonSet()
{
    /*this->points.resize(this->centers.size() * 8);

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

        this->points[(ix * 8) + 0] = (A1L);
        this->points[(ix * 8) + 1] = (OL);
        this->points[(ix * 8) + 2] = (OR);
        this->points[(ix * 8) + 3] = (A1R);

        this->points[(ix * 8) + 4] = (OL);
        this->points[(ix * 8) + 5] = (B2L);
        this->points[(ix * 8) + 6] = (B2R);
        this->points[(ix * 8) + 7] = (OR);
    }*/
}

void PlainRoadBuilder::buildSinglePolygon()
{
    int size = this->centers.size() * 2;
    this->points.resize(size);

    Vector2d _a;

    for (int i = 1; i < this->centers.size() - 1; i++)
    {
        Vector2d A = this->centers[i - 1],
                B = this->centers[i],
                C = this->centers[i + 1],
                BA = A - B;

        double angle = 0; //M_PI / 16.0;

        if (fabs((C - B).angleTo(A - B) - M_PI) < pow(10.0, -5.0))
        {
            angle = 0;
        }

        Vector2d a = BA.perpendicular().normalize().rotate(angle) * this->roadWidth;

        if (_a.dotProduct(a) < 0)
        {
            a = a * -1;
        }

        _a = a;

        Vector2d AL = A + a,
                AR = A - a;

        this->points[i - 1] = AL;
        this->points[size - i] = AR;
    }

    if (false)
    {
        Vector2d A = this->centers[this->centers.size() - 1],
                B = this->centers[this->centers.size() - 2],
                BA = A - B,
                a = BA.perpendicular().normalize().rotate(M_PI / 16.0) * this->roadWidth,
                AL = A + a,
                AR = A - a;

        this->points[size - 2] = AL;
        this->points[size - 1] = AR;
    }

    /*this->points.resize(this->centers.size() * 2);

    for (int i = 2; i < this->centers.size() - 2; i += 2)
    {
        QVector<Vector2d> sidePoints;
        Vector2d A, B, C;
        Vector2d A1L, A1R, B1L, B1R, B2L, B2R, C1L, C1R;

        A = centers[i - 2];
        B = centers[i - 1];
        C = centers[i - 0];

        sidePoints = this->getSidePoints(A, B, C);
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

        static int size = this->centers.size();

        this->points[(ix * 3) + 0] = A1L;
        this->points[(ix * 3) + 1] = OL;
        this->points[(ix * 3) + 2] = C1L;

        this->points[size - (ix * 3) + 0] = A1R;
        this->points[size - (ix * 3) + 1] = OR;
        this->points[size - (ix * 3) + 2] = C1R;
    }*/
}

void PlainRoadBuilder::buildLinesOnly()
{
    for (int i = 1; i < this->centers.size() - 1; i++)
    {
        Vector2d A = this->centers[i - 1],
                B = this->centers[i],
                C = this->centers[i + 1],
                BA = A - B;

        double angle = M_PI / 16.0;

        if (fabs((C - B).angleTo(A - B) - M_PI) < pow(10.0, -5.0))
        {
            angle = 0;
        }

        Vector2d a = BA.perpendicular().normalize().rotate(angle) * this->roadWidth,
                AL = A + a,
                AR = A - a;

        this->points.push_back(AL);
        this->points.push_back(AR);
    }

    {
        Vector2d A = this->centers[this->centers.size() - 1],
                B = this->centers[this->centers.size() - 2],
                BA = A - B,
                a = BA.perpendicular().normalize().rotate(M_PI / 16.0) * this->roadWidth,
                AL = A + a,
                AR = A - a;

        this->points.push_back(AL);
        this->points.push_back(AR);
    }
}

void PlainRoadBuilder::buildCentersOnly()
{
    for (int i = 0; i < this->centers.size(); i++)
    {
        this->points.push_back(centers[i]);
    }
}

void PlainRoadBuilder::build(int mode)
{
    switch (mode)
    {
        case POLYGON_SET:
            this->buildPolygonSet();
        break;

        case SINGLE_POLYGON:
            this->buildSinglePolygon();
        break;

        case LINES_ONLY:
            this->buildLinesOnly();
        break;

        case CENTERS_ONLY:
            this->buildCentersOnly();
        break;
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

QVector<Vector2d> PlainRoadBuilder::getSidePoints(Vector2d A, Vector2d B, Vector2d C)
{
    QVector<Vector2d> results;
    Vector2d AL, AR, CL, CR, OL, OR;

    Vector2d a = A - B,
            ap = a.perpendicular().normalize() * this->roadWidth;

    AL = A + ap;
    AR = A - ap;

    a = C - B;
    ap = a.perpendicular().normalize() * this->roadWidth;

    CL = C + ap;
    CR = C - ap;

    Vector2d BA = A - B,
            BC = C - B;

    double alpha = BA.angleTo(BC),
            phi = alpha / 2.0;

    OL = (BA.normalize().rotate(1 * (M_PI + phi)) * this->roadWidth) + B;
    OR = (BC.normalize().rotate(1 * (M_PI + phi)) * this->roadWidth) + B;

    results.push_back(AL);
    results.push_back(AR);
    results.push_back(OL);
    results.push_back(OR);
    results.push_back(CL);
    results.push_back(CR);

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
