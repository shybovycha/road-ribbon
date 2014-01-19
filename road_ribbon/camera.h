#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "vector3d.h"

class Camera
{
public:
    Camera();
    Camera(Vector3d _position, Vector3d _lookAt);

    void move(Vector3d translate);
    void rotate(Vector3d angles);
    void render();

    Vector3d position, lookAt, up, left;
};

#endif // CAMERA_H
