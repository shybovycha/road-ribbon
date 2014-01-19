#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <QDebug>
#include <QString>

#include "vector3d.h"

class Camera
{
public:
    Camera(Vector3d _position, Vector3d _lookAt);

    void move(Vector3d translate);
    void rotate(Vector3d angles);
    void render();

    Vector3d position, lookAt, up, left, forward;
};

#endif // CAMERA_H
