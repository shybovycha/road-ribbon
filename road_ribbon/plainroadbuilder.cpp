#include "plainroadbuilder.h"

double sec(double x)
{
    return 1.0 / cos(x);
}

PlainRoadBuilder::PlainRoadBuilder(QVector<SkeletonCenter> _centers, float roadWidth)
{
    if (_centers.size() < 3)
    {
        qDebug() << QString("Given %1 centers. That's not good - we can't build road that simple.").arg(_centers.size());
        return;
    }

    Vector2d origin = this->latlng2xy(_centers[0].location);

    for (int i = 1; i < _centers.size(); i++)
    {
        while ((i < _centers.size() - 1) && (_centers[i + 1].location == _centers[i].location)) i++;

        SkeletonCenter center = _centers[i];

        Vector2d xy = this->latlng2xy(_centers[i].location, origin);

        center.position = Vector3d(xy.x, _centers[i].position.y / ZOOM, xy.y);

        this->centers.push_back(center);
    }

    this->roadWidth = roadWidth;
}

QVector<sf::ConvexShape> PlainRoadBuilder::buildSinglePolygon()
{
    QVector<sf::ConvexShape> shapes;

    int size = this->centers.size() * 2;
    sf::ConvexShape convex = this->createShape();
    convex.setPointCount(size);

    Vector2d _a;

    for (int i = 1; i < this->centers.size() - 1; i++)
    {
        Vector2d A = this->centers[i - 1].planePos(),
                B = this->centers[i].planePos(),
                C = this->centers[i + 1].planePos(),
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

        convex.setPoint(i - 1, AL.toSFML());
        convex.setPoint(size - i, AR.toSFML());
    }

    if (false)
    {
        Vector2d A = this->centers[this->centers.size() - 1].planePos(),
                B = this->centers[this->centers.size() - 2].planePos(),
                BA = A - B,
                a = BA.perpendicular().normalize().rotate(M_PI / 16.0) * this->roadWidth,
                AL = A + a,
                AR = A - a;

        convex.setPoint(size - 2, AL.toSFML());
        convex.setPoint(size - 1, AR.toSFML());
    }

    shapes.push_back(convex);

    return shapes;
}

void PlainRoadBuilder::buildLinesOnly(QVector<sf::Vertex>* points, int *lineCount)
{
    int N = this->centers.size() * 2;
    *lineCount = N / 2;
    points->resize(N);
    BoundingBox bbox;

    for (int i = 1; i < this->centers.size() - 1; i++)
    {
        Vector2d A = this->centers[i - 1].planePos(),
                B = this->centers[i].planePos(),
                C = this->centers[i + 1].planePos(),
                BA = A - B;

        double angle = 0; // M_PI / 16.0;

        if (fabs((C - B).angleTo(A - B) - M_PI) < pow(10.0, -5.0))
        {
            angle = 0;
        }

        Vector2d a = BA.perpendicular().normalize().rotate(angle) * this->roadWidth,
                AL = A + a,
                AR = A - a;

        bbox.extend(AL);
        bbox.extend(AR);

        points->push_back(sf::Vertex(AL.toSFML()));
        points->push_back(sf::Vertex(AR.toSFML()));
    }

    {
        Vector2d A = this->centers[this->centers.size() - 1].planePos(),
                B = this->centers[this->centers.size() - 2].planePos(),
                BA = A - B,
                a = BA.perpendicular().normalize().rotate(M_PI / 16.0) * this->roadWidth,
                AL = A + a,
                AR = A - a;

        bbox.extend(AL);
        bbox.extend(AR);

        points->push_back(sf::Vertex(AL.toSFML()));
        points->push_back(sf::Vertex(AR.toSFML()));
    }
}

void PlainRoadBuilder::buildPolygonSet(QVector<Vector3d>* vertices, QVector<int>* indices)
{
    for (int i = 1; i < this->centers.size() - 1; i++)
    {
        Vector2d A = this->centers[i - 1].planePos(),
                B = this->centers[i].planePos(),
                C = this->centers[i + 1].planePos(),
                BA = A - B;

        double angle = 0; // M_PI / 16.0;

        if (fabs((C - B).angleTo(A - B) - M_PI) < pow(10.0, -5.0))
        {
            angle = 0;
        }

        Vector2d a = BA.perpendicular().normalize().rotate(angle) * this->roadWidth,
                AL = A + a,
                AR = A - a;

        Vector3d AL3D = Vector3d(AL.x, this->centers[i].position.y, AL.y),
                AR3D = Vector3d(AR.x, this->centers[i].position.y, AR.y);

        int index = vertices->size();

        if (i == 1)
        {
            vertices->push_back(AL3D);
            vertices->push_back(AR3D);
        } else
        {
            vertices->push_back(AR3D);
            vertices->push_back(AL3D);
            vertices->push_back(AL3D);
            vertices->push_back(AR3D);

            indices->push_back(index - 2);
            indices->push_back(index - 1);
            indices->push_back(index + 0);
            indices->push_back(index + 1);
        }
    }

    {
        Vector2d A = this->centers[this->centers.size() - 1].planePos(),
                B = this->centers[this->centers.size() - 2].planePos(),
                BA = A - B,
                a = BA.perpendicular().normalize().rotate(M_PI / 16.0) * this->roadWidth,
                AL = A + a,
                AR = A - a;

        Vector3d AL3D = Vector3d(AL.x, this->centers[this->centers.size() - 1].position.y, AL.y),
                AR3D = Vector3d(AR.x, this->centers[this->centers.size() - 1].position.y, AR.y);

        vertices->push_back(AL3D);
        vertices->push_back(AR3D);
    }

    /**primitiveCount = this->centers.size() - 1;

    for (int i = 1; i < this->centers.size() - 1; i++)
    {
        Vector3d A = this->centers[i - 1].position,
                B = this->centers[i].position,
                BA = A - B;

        Vector2d BAplanar(BA.x, BA.z),
                BAperp = BAplanar.perpendicular();

        Vector3d a = (Vector3d(BAperp.x, 0, BAperp.y)).normalize() * this->roadWidth,
                AL = A + a,
                AR = A - a;

        AR.y = 0;
        AL.y = 0;

        int index = vertices->size();

        if (i == 1)
        {
            vertices->push_back(AL);
            vertices->push_back(AR);

            indices->push_back(index + 0);
            indices->push_back(index + 1);
        } else
        {
            vertices->push_back(AR);
            vertices->push_back(AL);
            vertices->push_back(AL);
            vertices->push_back(AR);

            indices->push_back(index - 2);
            indices->push_back(index - 1);
            indices->push_back(index + 0);
            indices->push_back(index + 1);
        }
    }

    {
        Vector3d A = this->centers[this->centers.size() - 1].position,
                B = this->centers[this->centers.size() - 2].position,
                BA = A - B;

        Vector2d BAplanar(BA.x, BA.z),
                BAperp = BAplanar.perpendicular();

        Vector3d a = (Vector3d(BAperp.x, 0, BAperp.y)).normalize() * this->roadWidth,
                AL = A + a,
                AR = A - a;

        AR.y = 0;
        AL.y = 0;

        vertices->push_back(AR);
        vertices->push_back(AL);
    }*/
}

void PlainRoadBuilder::render(int mode, sf::RenderTarget *window)
{
    if (mode == SINGLE_POLYGON)
    {
        QVector<sf::ConvexShape> shapes = this->buildSinglePolygon();
    }

    if (mode == LINES_ONLY)
    {
        QVector<sf::Vertex> points;
        int lineCount;
        this->buildLinesOnly(&points, &lineCount);

        for (int i = 0; i < points.size() - 2; i += 2)
        {
            sf::Vertex line[] = {
                points[i],
                points[i + 1]
            };

            window->draw(line, 2, sf::Lines);
        }
    }

    if (mode == SURFACE)
    {
        static QVector<Vector3d> vertices;
        static QVector<int> indices;

        if (vertices.empty())
        {
            this->buildPolygonSet(&vertices, &indices);
        }

        GLfloat *glvertices = 0;
        GLuint *glindices = 0;

        toGLVertexArray(vertices, indices, glvertices, glindices);

        glMatrixMode(GL_MODELVIEW);

        glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, glvertices);

        glColor3f(0.5, 0.5, 0.7);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, glindices);

        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
}

sf::ConvexShape PlainRoadBuilder::createShape()
{
    sf::ConvexShape convex;

    convex.setFillColor(sf::Color(0, 0, 0, 0));
    convex.setOutlineColor(sf::Color(100, 100, 150));
    convex.setOutlineThickness(1);

    return convex;
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

void PlainRoadBuilder::toGLVertexArray(const QVector<Vector3d> &vertices, const QVector<int> &indices, GLfloat *&glvertices, GLuint *&glindices)
{
    glvertices = new GLfloat[vertices.size() * 3];
    glindices = new GLuint[indices.size()];

    int index = 0;

    for (int i = 0; i < vertices.size(); i++)
    {
        glvertices[index++] = vertices[i].x;
        glvertices[index++] = vertices[i].y;
        glvertices[index++] = vertices[i].z;
    }

    index = 0;

    for (int i = 0; i < indices.size(); i++)
    {
        glindices[index++] = indices[i];
    }
}
