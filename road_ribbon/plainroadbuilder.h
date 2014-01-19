#ifndef PLAINROADBUILDER_H
#define PLAINROADBUILDER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <GL/gl.h>

#include <QVector>
#include <QDebug>

#include "vector2d.h"
#include "vector3d.h"
#include "skeletoncenter.h"
#include "boundingbox.h"

class PlainRoadBuilder
{
protected:
    Vector2d latlng2xy(Vector2d latlng, Vector2d origin = Vector2d(0, 0));
    QVector<Vector2d> getSidePoints(Vector2d A, Vector2d B, Vector2d C);
    Vector2d getCrossPoint(Vector2d p1, Vector2d p2, Vector2d p3, Vector2d p4);
    QVector<sf::ConvexShape> buildSinglePolygon();
    void buildPolygonSet(QVector<Vector3d>* vertices, QVector<int>* indices);
    void buildLinesOnly(QVector<sf::Vertex>* points, int *lineCount);
    sf::ConvexShape createShape();
    void toGLVertexArray(const QVector<Vector3d> &vertices, const QVector<int> &indices, GLfloat *&glvertices, GLuint *&glindices);

    QVector<SkeletonCenter> centers;

    float roadWidth;

    const static float ZOOM = 7.0;
    const static float DEFAULT_ROAD_WIDTH = 15.0;

public:
    PlainRoadBuilder(QVector<SkeletonCenter> centers, float roadWidth = DEFAULT_ROAD_WIDTH);

    void render(int mode, sf::RenderTarget *window);

    enum RenderMode { POLYGON_SET, SINGLE_POLYGON, LINES_ONLY, SURFACE };
};

#endif // PLAINROADBUILDER_H
